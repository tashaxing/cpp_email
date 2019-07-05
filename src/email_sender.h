#ifndef _EMAIL_SENDER_H_
#define	_EMAIL_SENDER_H_
 
#include <string>
#include <vector>
#include <curl/curl.h>
 
#define MULTI_PERFORM_HANG_TIMEOUT 60 * 1000
 
 
class EmailSender 
{
public:
    EmailSender();
    EmailSender(  //create sendmail object with paremeter;
                const std::string & strUser,
                const std::string & strPsw, 
                const std::string & strSmtpServer, 
                int iPort, 
                const std::string & strMailFrom
            );
    EmailSender(const EmailSender& orig);
    virtual ~EmailSender();
private:
    std::string m_strUser;			//邮箱用户名
    std::string m_strPsw;			//邮箱密码
    std::string m_strSmtpServer;		//邮箱SMTP服务器
    int         m_iPort;			//邮箱SMTP服务器端口
    std::vector<std::string> m_RecipientList;  	//接收者邮件list
    std::vector<std::string> m_CcRecipientList; //抄送人尤佳list
    std::string m_strMailFrom;			//发送者邮箱
    std::vector<std::string> m_attachments; // 附件
    std::vector<std::string> m_MailContent;	//发送的内容队列，包括头和内容项
    int         m_iMailContentPos;		//用于发送数据时记录发送到第几个content
    
private:
    //发送内容回调函数
    static size_t read_callback(void *ptr, size_t size, size_t nmemb, void *userp);
    //获取当前时间
    static struct timeval tvnow(void);
    //两个时间差
    static long tvdiff(struct timeval newer, struct timeval older);
    //创建邮件内容
    bool ConstructHead(const std::string & strSubject/*邮件主题*/, const std::string & strContent/*邮件内容*/);
    
public:
    
    bool SendMail(const std::string & strSubject, const char * pMailBody, int len);
    bool SendMail(const std::string & strSubject, const std::string & strMailBody);
    bool SendMail(  //create sendmail object with paremeter;
                    const std::string & strUser,
                    const std::string & strPsw, 
                    const std::string & strSmtpServer, 
                    int iPort, 
                    std::vector<std::string> & recipientList,
                    const std::string & strMailFrom,
                    const std::string & strSubject, 
                    const char * pMailBody, 
                    int len
                );
    bool SendMail(  //create sendmail object with paremeter;
                    const std::string & strUser,
                    const std::string & strPsw, 
                    const std::string & strSmtpServer, 
                    int iPort, 
                    const std::string & strMailTo,
                    const std::string & strMailFrom,
                    const std::string & strSubject, 
                    const char * pMailBody, 
                    int len
                );
    bool SendMail(  //create sendmail object with paremeter;
                    const std::string & strUser,
                    const std::string & strPsw, 
                    const std::string & strSmtpServer, 
                    int iPort, 
                    std::vector<std::string> & recipientList,
                    const std::string & strMailFrom,
                    const std::string & strSubject, 
                    const std::string & strMailBody
                );
    bool SendMail(  //create sendmail object with paremeter;
                    const std::string & strUser,
                    const std::string & strPsw, 
                    const std::string & strSmtpServer, 
                    int iPort, 
                    const std::string & strMailTo,
                    const std::string & strMailFrom,
                    const std::string & strSubject, 
                    const std::string & strMailBody
                );
    
    void SetUser(const std::string & strUser) { m_strUser = strUser; }
    std::string & GetUser() { return m_strUser; }
    
    void SetPsw(const std::string & strPsw) { m_strPsw = strPsw; }
    std::string & GetPsw() { return m_strPsw; }
    
    void SetSmtpServer(const std::string & strSmtpServer) { m_strSmtpServer = strSmtpServer; }
    std::string & GetSmtpServer() { return m_strSmtpServer; }
    
    void SetPort(int iPort) { m_iPort = iPort; }
    int GetPort() { return m_iPort; }
    
    void SetMailFrom(const std::string & strMailFrom) { m_strMailFrom = strMailFrom; }
    std::string & GetMailFrom() { return m_strMailFrom; }
    
    //添加接收者邮箱
    void AddRecipient(const std::string & strMailTo) { m_RecipientList.push_back(strMailTo); }
    void AddRecipient(std::vector<std::string> recipientList) 
    { 
        std::copy(recipientList.begin(), recipientList.end(), m_RecipientList.begin());
    }
    void AddCcRecipient(const std::string& strCcTo) { m_CcRecipientList.push_back(strCcTo);}
    void AddCcRecipient(std::vector<std::string> ccRecipientList)
    {
    	std::copy(ccRecipientList.begin(), ccRecipientList.end(), m_CcRecipientList.begin());
    }

    void ClearRecipient() { m_RecipientList.clear(); }
    void ClearCcRecipient() { m_CcRecipientList.clear();}
    
};
 
 
#endif	/* _EMAIL_SENDER_H_ */