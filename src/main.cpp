#include <iostream>
#include "email_sender.h"

int main(int argc, char** argv)
{
	std::cout << "curl email test" << std::endl;

	EmailSender email_sender("aaaa@126.com", "xxxx", "smtp.126.com", 25, "<aaaa@126.com>");
	email_sender.AddRecipient("<bbbb@163.com>"); // FIXME: must use this format ?
	email_sender.AddRecipient("<cccc@163.com>");
	email_sender.AddCcRecipient("<dddd@126.com>");
	email_sender.AddCcRecipient("<ffff@126.com>");

	email_sender.SendMail("my title", "hello C++ email");

	return 0;
}