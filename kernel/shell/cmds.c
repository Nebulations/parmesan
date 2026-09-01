extern void scmd_register_echo(void);
extern void scmd_register_clear(void);
extern void scmd_register_test(void);

void scmds_init() {
    scmd_register_echo();
    scmd_register_clear();
    scmd_register_test();
}