extern void scmd_register_echo();
extern void scmd_register_clear();
extern void scmd_register_test();
extern void scmd_register_res();

void scmds_init() {
    scmd_register_echo();
    scmd_register_clear();
    scmd_register_test();
    scmd_register_res();
}