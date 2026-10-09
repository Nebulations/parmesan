extern void scmd_register_echo();
extern void scmd_register_clear();
extern void scmd_register_test();
extern void scmd_register_res();

// FS related commands
extern void scmd_register_ls();
extern void scmd_register_cd();
extern void scmd_register_wai();

// OS basics
extern void scmd_register_uptime();
extern void scmd_register_sleep();

void scmds_init() {
    scmd_register_echo();
    scmd_register_clear();
    scmd_register_test();
    scmd_register_res();

    scmd_register_ls();
    scmd_register_cd();
    scmd_register_wai();

    scmd_register_uptime();
    scmd_register_sleep();
}