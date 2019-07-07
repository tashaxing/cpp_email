#include "email_sender.h"

EmailSender::EmailSender(const std::string& smtp_server,
                         const int smtp_port,
                         const std::string& from_email,
                         const std::string& password,
                         const std::string& from_name)
{
    // init settings
    m_smtp_url = "smtp://" + smtp_server + ':' + std::to_string(smtp_port); // smtp://example.com
    m_from = std::make_pair(from_email, from_name);
    m_password = password;
}

EmailSender::~EmailSender()
{
    m_recvs.clear();
    m_ccs.clear();
    m_attachments.clear();
}

void EmailSender::setEmailContent(const std::string& subject,
                                  const std::string& body)
{
    // set email title and body content
    m_email_subject = subject;
    m_email_body = body;
}

void EmailSender::addRecvEmailAddr(const std::string& email_addr, const std::string &name)
{
    // add receiver to list
    m_recvs.push_back(std::make_pair(email_addr, name));
}

void EmailSender::addCcEmailAddr(const std::string& email_addr, const std::string &name)
{
    // add cc to list
    m_ccs.push_back(std::make_pair(email_addr, name));
}

void EmailSender::addAttachment(const std::string& filename)
{
    // add attach filename to list
    m_attachments.push_back(filename); // NOTICE: can be absolute or relative file path
}

void EmailSender::send()
{
    // here init curl and do smtp email send
    CURL *curl;
    CURLcode res = CURLE_OK;

    curl = curl_easy_init();
    if(curl)
    {
        struct curl_slist *headers = NULL;
        struct curl_slist *recipients = NULL;
        struct curl_slist *slist = NULL;
        curl_mime *mime;
        curl_mime *alt;
        curl_mimepart *part;

        /* This is the URL for your mailserver */
        curl_easy_setopt(curl, CURLOPT_URL, m_smtp_url.c_str());

        /* Login smtp server to verify */
        curl_easy_setopt(curl, CURLOPT_USERNAME, m_from.first.c_str());
        curl_easy_setopt(curl, CURLOPT_PASSWORD, m_password.c_str());

        /* Note that this option isn't strictly required, omitting it will result
       * in libcurl sending the MAIL FROM command with empty sender data. All
       * autoresponses should have an empty reverse-path, and should be directed
       * to the address in the reverse-path which triggered them. Otherwise,
       * they could cause an endless loop. See RFC 5321 Section 4.5.5 for more
       * details.
       */
        std::string from_email_addr = '<' + m_from.first + '>'; // should be like <example@126.com>
        curl_easy_setopt(curl, CURLOPT_MAIL_FROM, from_email_addr.c_str());

        /* Add recipients, in this particular case they correspond to the
       * To: and Cc: addressees in the header, but they could be any kind of
       * recipient. */
        // receiver
        for (auto &email_pair : m_recvs)
        {
            std::string email_addr = '<' + email_pair.first + '>';
            recipients = curl_slist_append(recipients, email_addr.c_str());
        }
        // cc
        for (auto &email_pair : m_ccs)
        {
            std::string email_addr = '<' + email_pair.first + '>';
            recipients = curl_slist_append(recipients, email_addr.c_str());
        }
        curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);

        /* Build and set the message header list. */
        std::string header;

        header += "From:";
        header += m_from.second + '<' + m_from.first + '>' + "\n";

        header += "To:";
        for (int i = 0; i < m_recvs.size(); i++)
        {
            header += m_recvs[i].second += '<' + m_recvs[i].first + '>';
            if (i != m_recvs.size() - 1)
                header += ',';
        }
        header += "\n";

        header += "Cc:";
        for (int i = 0; i < m_ccs.size(); i++)
        {
            header += m_ccs[i].second += '<' + m_ccs[i].first + '>';
            if (i != m_ccs.size() - 1)
                header += ',';
        }
        header += "\n";

        header += "Subject:";
        header += m_email_subject + "\n";

//        printf("email header:\n");
//        printf(header.c_str());

        headers = curl_slist_append(headers, header.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        /* Build the mime message. */
        mime = curl_mime_init(curl);

        /* The inline part is an alternative proposing the html and the text
         versions of the e-mail. */
        alt = curl_mime_init(curl);

        /* HTML message. */
        part = curl_mime_addpart(alt);
        curl_mime_data(part, m_email_body.c_str(), CURL_ZERO_TERMINATED);
        curl_mime_type(part, "text/html");

        /* Text message. */
        //    part = curl_mime_addpart(alt);
        //    curl_mime_data(part, m_email_body.c_str(),, CURL_ZERO_TERMINATED);

        /* Create the inline part. */
        part = curl_mime_addpart(mime);
        curl_mime_subparts(part, alt);
        curl_mime_type(part, "multipart/alternative");
        slist = curl_slist_append(NULL, "Content-Disposition: inline");
        curl_mime_headers(part, slist, 1);

        /* Add the attachments */
        for (std::string &filename : m_attachments)
        {
            part = curl_mime_addpart(mime);
            curl_mime_filedata(part, filename.c_str());
            curl_easy_setopt(curl, CURLOPT_MIMEPOST, mime);
        }

        /* Send the message */
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1); // 1 means open info print, 0 not
        res = curl_easy_perform(curl);

        /* Check for errors */
        if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));

        /* Free lists. */
        curl_slist_free_all(recipients);
        curl_slist_free_all(headers);

        /* curl won't send the QUIT command until you call cleanup, so you should
       * be able to re-use this connection for additional messages (setting
       * CURLOPT_MAIL_FROM and CURLOPT_MAIL_RCPT as required, and calling
       * curl_easy_perform() again. It may not be a good idea to keep the
       * connection open for a very long time though (more than a few minutes
       * may result in the server timing out the connection), and you do want to
       * clean up in the end.
       */
        curl_easy_cleanup(curl);

        /* Free multipart message. */
        curl_mime_free(mime);
    }
}
