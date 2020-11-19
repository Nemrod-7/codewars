#include <string>
#include <vector>

enum state {CLOSED,LISTEN,SYN_SENT,SYN_RCVD,ESTABLISHED,FIN_WAIT_1,CLOSE_WAIT,CLOSING,TIME_WAIT,FIN_WAIT_2,LAST_ACK};
std::vector<std::string> state  {"CLOSED","LISTEN","SYN_SENT","SYN_RCVD","ESTABLISHED","FIN_WAIT_1",
 "CLOSE_WAIT","CLOSING","TIME_WAIT","FIN_WAIT_2","LAST_ACK",};

std::string traverse_TCP_states(const std::vector<std::string> &events) {

    int status = CLOSED;
    for (auto com : events) {
        switch (status) {
            case CLOSED : {
                if (com == "APP_PASSIVE_OPEN") status = LISTEN;
                else if (com == "APP_ACTIVE_OPEN") status = SYN_SENT;
                else return "ERROR";
                break;}
            case LISTEN : {
                if (com == "RCV_SYN") status = SYN_RCVD;
                else if (com == "APP_SEND") status = SYN_SENT;
                else if (com == "APP_CLOSE") status = CLOSED;
                else return "ERROR";
                break;}
            case SYN_SENT : {
                if (com == "RCV_SYN") status = SYN_RCVD;
                else if (com == "RCV_SYN_ACK") status = ESTABLISHED;
                else if (com == "APP_CLOSE") status = CLOSED;
                else return "ERROR";
                break;}
            case SYN_RCVD : {
                if (com == "APP_CLOSE") status = FIN_WAIT_1;
                else if (com == "RCV_ACK") status = ESTABLISHED;
                else return "ERROR";
                break;}
            case ESTABLISHED : {
                if (com == "APP_CLOSE") status = FIN_WAIT_1;
                else if (com == "RCV_FIN") status = CLOSE_WAIT;
                else return "ERROR";
                break;}
            case FIN_WAIT_1 : {
                if (com == "RCV_FIN") status = CLOSING;
                else if (com == "RCV_FIN_ACK") status = TIME_WAIT;
                else if (com == "RCV_ACK") status = FIN_WAIT_2;
                else return "ERROR";
                break;}
            case CLOSE_WAIT : {
                if (com == "APP_CLOSE") status = LAST_ACK;
                else return "ERROR";
                break;}
            case CLOSING : {
                if (com == "RCV_ACK") status = TIME_WAIT;
                else return "ERROR";
                break;}
            case TIME_WAIT : {
                if (com == "APP_TIMEOUT") status = CLOSED;
                else return "ERROR";
                break;}
            case FIN_WAIT_2 : {
                if (com == "RCV_FIN") status = TIME_WAIT;
                else return "ERROR";
                break;}
            case LAST_ACK : {
                if (com == "RCV_ACK") status = CLOSED;
                else return "ERROR";
                break;}
        }

    }
    return state[status];
}
