This project contains upper layer or application regarding Kineis stack.

Two applications are available in KNS_APP files:
* A standalone app (\ref KNS_APP_stdalone) is sending one message
* a GUI app (\ref KNS_APP_gui) is dedicated to communicate with the Kinéis Device Interface (KDI)
through AT commands (\ref mgr_at_cmd_page) over an UART link.

The format of an AT command is like "<pattern>=?\r\n" (e.g. "AT+PING=?\r\n") to get information or
"<pattern>=<params>\r\n" (e.g. "AT+TX=<user_data>\r\n") to set information. <br>
The list of supported AT commands is available through \ref cas_atcmd_list_array.

By the end, those applications shows how to use the Kinéis library (libkineis.a) which currently
exposes the `Service` layer of the Kinéis software stack, especially the `TX service` (kns_tx.h) and
the `configuration service` (kns_cfg.h).
