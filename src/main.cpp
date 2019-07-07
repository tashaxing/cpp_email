#include <iostream>
#include "email_sender.h"

int main(int arc, char* argv[])
{
    std::string smtp_server = "smtp.example.com"; // like smtp.126.com
    int smtp_port = 25;

    std::string from_email = "example@126.com"; // your email
    std::string password = "password"; // your password

    std::string to_email1 = "recv1@163.com";
    std::string to_name1 = "jack";
    std::string to_email2 = "recv2@live.com";
    std::string to_name2 = "tom";

    std::string cc_email1 = "cc1@126.com";
    std::string cc_name1 = "lucy";
    std::string cc_email2 = "cc2@qq.com";
    std::string cc_name2 = "lily";

    std::string email_subject = "test email";
    std::string email_body = "<html><body>\r\n"
                             "<p>This is the inline <b>HTML</b> message of the e-mail.</p>"
                             "<br />\r\n"
                             "<p>It could be a lot of HTML data that would be displayed by "
                             "e-mail viewers able to handle HTML.</p>"
                             "<br />\r\n"
                             "<p>thanks for watching, <font color=red>good luck</font></p>"
                             "</body></html>\r\n"; // html format

//    std::string email_body = "This is the inline text message of the e-mail.\r\n"
//                             "\r\n"
//                             "  It could be a lot of lines that would be displayed in an e-mail\r\n"
//                             "viewer that is not able to handle HTML.\r\n"
//                             "\r\n"
//                             "thanks for watching, good luck"
//                             "\r\n"; // plain text

    std::string filename1 = "attach1.txt";
    std::string filename2 = "attach2.log";

    EmailSender email_sender(smtp_server, smtp_port, from_email, password, "tashaxing");
    email_sender.addRecvEmailAddr(to_email1, to_name1);
    email_sender.addRecvEmailAddr(to_email2, to_name2);
    email_sender.addCcEmailAddr(cc_email1, cc_name1);
    email_sender.addCcEmailAddr(cc_email2, cc_name2);
    email_sender.setEmailContent(email_subject, email_body);
    email_sender.addAttachment(filename1);
    email_sender.addAttachment(filename2);
    email_sender.send();

    return 0;
}
