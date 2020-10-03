
#include "eehandler.h"
#include "eelog.h"
#include "bic.h"
#include "msgid.h"

const std::string INI_STRING = R"INI(

; GLOBAL options

; DBUG INFO WARN ERRO
LogLevel=DBUG
LogDir=./
LogSize=5

; `on` means whether start this process or not
; this would instruct current process to run as which behavior tagged by followed as-value:
;   `daemon` means run as a daemon process
;   `child`  means run as a child process created by a daemon
;   `server` means run as a independent tcp server 
;   `client` means run as a independent tcp client
[GUARD-DAEMON]
on=yes
as=daemon

[STEP-1]
on=yes
as=child

[STEP-2]
on=yes
as=child

[STEP-3]
on=yes
as=child

)INI";

/** IPC between sub process */
void step_1_function(const uint16_t msgid, const uint64_t origin, const uint64_t orient, const std::string& msg, void* arg);
void step_2_function(const uint16_t msgid, const uint64_t origin, const uint64_t orient, const std::string& msg, void* arg);
void step_3_function(const uint16_t msgid, const uint64_t origin, const uint64_t orient, const std::string& msg, void* arg);

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;
    
    std::string ini("guard.ini");
    
    std::ofstream ofs(ini.c_str(), std::ofstream::out | std::ofstream::binary);
    if(! ofs.is_open()) {
        ECHO(ERRO, "open %s for writing failed", ini.c_str());
    }
    
    std::istringstream iss(INI_STRING);
    
    ofs << iss.rdbuf();
    ofs.close();

    tcw::EventHandler eeh;
    tcw::RetCode rescode;

    tcw::EventHandler::tcw_register_service("STEP-1", step_1_function);
    tcw::EventHandler::tcw_register_service("STEP-2", step_2_function);
    tcw::EventHandler::tcw_register_service("STEP-3", step_3_function);

    rescode = eeh.tcw_init(ini.c_str());
    if (rescode != tcw::OK) {
        ECHO(ERRO, "tcw_init failed");
        return -1;
    }

    std::thread th([&eeh](){
        ECHO(DBUG, "thread(tid=%lu) sleep for 6 seconds and wait for child process start", (uint64_t)pthread_self());
        sleep(6);

        int counter = 100;
        while (counter--) {
            uint64_t tosid = eeh.tcw_get_sid("STEP-1");
            BIC_A2A_START bicstart;
            bicstart.is_start = true;
            bicstart.information = "create a message and ready to send";

            std::string msg;
            bicstart.Serialize(&msg);

            ECHO(DBUG, "send a start message to(sid=%lu)", tosid);
            eeh.tcw_send_message(BIC_TYPE_A2A_START, tosid, msg);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    });
    th.detach();

    eeh.tcw_run();

    eeh.tcw_destroy();

    return 0;
}

void step_1_function(const uint16_t msgid, const uint64_t origin, const uint64_t orient, const std::string& msg, void* arg)
{
    (void) origin;
    (void) orient;

    tcw::EventHandler *eeh = (tcw::EventHandler *)arg;
    /** deal with the message, defined by programmer */
    switch (msgid) {
        case BIC_TYPE_A2A_START:
        {
            BIC_A2A_START bic;
            bic.Structuralize(msg);

            Dbug(eeh->logger, TEST, "BIC_A2A_START.is_start: %d", bic.is_start);
            Dbug(eeh->logger, TEST, "BIC_A2A_START.information: %s", bic.information.c_str());

            BIC_A2B_BETWEEN bic_a2b;
            bic_a2b.send = true;
            bic_a2b.information = "send command to NEXT service";
            
            uint16_t tomsgid = BIC_TYPE_A2B_BETWEEN;
            uint64_t tosid = eeh->tcw_get_sid("STEP-2");
            std::string tomsg;
            bic_a2b.Serialize(&tomsg);

            eeh->tcw_send_message(tomsgid, tosid, tomsg);
            ECHO(INFO, "消息由 %s 服务向 %s 服务发送...", eeh->m_services_id[eeh->m_id].c_str(), eeh->m_services_id[tosid].c_str());
        }
        break;
        default:
            Erro(eeh->logger, TEST, "undefined or unhandled msg(%d)", (int)msgid);
    }
}

void step_2_function(const uint16_t msgid, const uint64_t origin, const uint64_t orient, const std::string& msg, void* arg)
{
    (void) origin;
    (void) orient;
    tcw::EventHandler *eeh = (tcw::EventHandler *)arg;
    /** deal with the message, defined by programmer */
    switch (msgid) {
        case BIC_TYPE_A2B_BETWEEN:
        {
            BIC_A2B_BETWEEN bic;
            bic.Structuralize(msg);
            
            Dbug(eeh->logger, TEST, "BIC_A2B_BETWEEN.send: %d", bic.send);
            Dbug(eeh->logger, TEST, "BIC_A2B_BETWEEN.information: %s", bic.information.c_str());
            
            BIC_B2C_BETWEEN bic_b2c;
            bic_b2c.send = true;
            bic_b2c.information = "send command to NEXT service";
            
            uint16_t tomsgid = BIC_TYPE_B2C_BETWEEN;
            uint64_t tosid = eeh->tcw_get_sid("STEP-3");
            std::string tomsg;
            bic_b2c.Serialize(&tomsg);

            eeh->tcw_send_message(tomsgid, tosid, tomsg);
            ECHO(INFO, "消息由 %s 服务向 %s 服务发送...", eeh->m_services_id[eeh->m_id].c_str(), eeh->m_services_id[tosid].c_str());
        }
        break;
        default:
            Erro(eeh->logger, TEST, "undefined or unhandled msg(%d)", (int)msgid);
    }
}
void step_3_function(const uint16_t msgid, const uint64_t origin, const uint64_t orient, const std::string& msg, void* arg)
{
    (void) origin;
    (void) orient;
    tcw::EventHandler *eeh = (tcw::EventHandler *)arg;
    /** deal with the message, defined by programmer */
    switch (msgid) {
        case BIC_TYPE_B2C_BETWEEN:
        {
            BIC_B2C_BETWEEN bic;
            bic.Structuralize(msg);
            
            Dbug(eeh->logger, TEST, "BIC_B2C_BETWEEN.send: %d", bic.send);
            Dbug(eeh->logger, TEST, "BIC_B2C_BETWEEN.information: %s", bic.information.c_str());
            
            ECHO(INFO, "%s 接收到来自 %s 的消息，一个流程结束。", eeh->m_services_id[eeh->m_id].c_str(), eeh->m_services_id[origin].c_str());
        }
        break;
        default:
            Erro(eeh->logger, TEST, "undefined or unhandled msg(%d)", (int)msgid);
    }
}