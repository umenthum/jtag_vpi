#ifndef __JTAG_SERVER_H__
#define __JTAG_SERVER_H__

#include "systemc.h"
#include "jtag_common.h"
#define XFERT_MAX_SIZE		512

#define DONE			0
#define IN_PROGRESS		1

#define CMD_RESET		0
#define CMD_TMS_SEQ		1
#define CMD_SCAN_CHAIN		2
#define CMD_SCAN_CHAIN_FLIP_TMS	3
#define CMD_STOP_SIMU		4

#define CHECK_CMD		0
#define TAP_RESET		1
#define GOTO_IDLE		2
#define DO_TMS_SEQ		3
#define SCAN_CHAIN		4
#define FINISHED		5

SC_MODULE(SystemCJtagServer) {
public:
	SC_CTOR(SystemCJtagServer);
	~SystemCJtagServer();

	void doJTAG();
	int init_jtag_server(int port);

    sc_in <bool> clk;
    sc_in <bool> rstn; //TODO handle tap reset
    sc_out<bool> tck;
    sc_out<bool> tms;
    sc_out<bool> tdi;
    sc_in <bool> tdo;
    sc_out<bool> trstn;

private:
	int gen_clk(int nb_period, uint8_t *captured_tdo, int restart, int get_tdo);
	void gen_clk_restart(void);
	int reset_tap();
	int goto_run_test_idle();
	int do_tms_seq(int length, int nb_bits, unsigned char *buffer);
	int do_scan_chain(int length, int nb_bits, unsigned char *buffer_out,
			  unsigned char *buffer_in, int flip_tms);

	struct jtag_cmd packet;

	int jtag_state;
	int cmd_in_progress;
	int tms_flip;
	uint64_t tck_period;
};

#endif
