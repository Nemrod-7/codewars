#include <stdio.h>

enum event { APP_PASSIVE_OPEN, APP_ACTIVE_OPEN, APP_SEND, APP_CLOSE, APP_TIMEOUT, RCV_SYN, RCV_ACK, RCV_SYN_ACK, RCV_FIN, RCV_FIN_ACK };
enum state { ERROR, CLOSED, LISTEN, SYN_SENT, SYN_RCVD, ESTABLISHED, CLOSE_WAIT, LAST_ACK, FIN_WAIT_1, FIN_WAIT_2, CLOSING, TIME_WAIT, };

enum state get_TCP_state (size_t n, const enum event events[n]) {

  enum state status = CLOSED;
  for (int i = 0; i < n; i++) {
      enum event com = events[i];

      switch (status) {
          case CLOSED : {
              if (com == APP_PASSIVE_OPEN) status = LISTEN;
              else if (com == APP_ACTIVE_OPEN) status = SYN_SENT;
              else status = ERROR;
              break;
          }
          case LISTEN : {
              if (com == RCV_SYN) status = SYN_RCVD;
              else if (com == APP_SEND) status = SYN_SENT;
              else if (com == APP_CLOSE) status = CLOSED;
              else status = ERROR;
              break;
          }
          case SYN_RCVD : {
              if (com == APP_CLOSE) status = FIN_WAIT_1;
              else if (com == RCV_ACK) status = ESTABLISHED;
              else status = ERROR;
              break;
          }
          case SYN_SENT : {
              if (com == RCV_SYN) status = SYN_RCVD;
              else if (com == RCV_SYN_ACK) status = ESTABLISHED;
              else if (com == APP_CLOSE) status = CLOSED;
              else status = ERROR;
              break;
          }
          case ESTABLISHED : {
              if (com == APP_CLOSE) status = FIN_WAIT_1;
              else if (com == RCV_FIN) status = CLOSE_WAIT;
              else status = ERROR;
              break;
          }
          case FIN_WAIT_1 : {
              if (com == RCV_FIN) status = CLOSING;
              else if (com == RCV_FIN_ACK) status = TIME_WAIT;
              else if (com == RCV_ACK) status = FIN_WAIT_2;
              else status = ERROR;
              break;
          }
          case CLOSING : {
              if (com == RCV_ACK) status = TIME_WAIT;
              else status = ERROR;
              break;
          }
          case FIN_WAIT_2 : {
              if (com == RCV_FIN) status = TIME_WAIT;
              else status = ERROR;
              break;
          }
          case TIME_WAIT : {
              if (com == APP_TIMEOUT) status = CLOSED;
              else status = ERROR;
              break;
          }
          case CLOSE_WAIT : {
              if (com == APP_CLOSE) status = LAST_ACK;
              else status = ERROR;
              break;
          }
          case LAST_ACK : {
              if (com == RCV_ACK) status = CLOSED;
              else status = ERROR;
              break;
          }
      }

  }

	return status;
}

int main () {


}
