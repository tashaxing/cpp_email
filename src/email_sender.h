#ifndef _EMAIL_SENDER_H_
#define	_EMAIL_SENDER_H_
 
#include <string>
#include <vector>
#include <utility>

#include "curl/curl.h"

class EmailSender 
{
public:
    EmailSender(const std::string& smtp_server,
                const int smtp_port,
                const std::string& from_email,
                const std::string& password,
                const std::string& from_name="");
    ~EmailSender();
    void setEmailContent(const std::string& subject="",
                         const std::string& body=""); // body can be html format or plain text
    void addRecvEmailAddr(const std::string& email_addr, const std::string& name="");
    void addCcEmailAddr(const std::string& email_addr, const std::string& name="");
    void addAttachment(const std::string& filename);
    void send();

private:
    std::string m_smtp_url;
    std::pair<std::string, std::string> m_from;
    std::string m_password;
    std::vector<std::pair<std::string, std::string>> m_recvs; // list of (email,name)
    std::vector<std::pair<std::string, std::string>> m_ccs; // list of (email,name)
    std::string m_email_subject;
    std::string m_email_body;
    std::vector<std::string> m_attachments; // list of filename
};
 
 
#endif	/* _EMAIL_SENDER_H_ */
