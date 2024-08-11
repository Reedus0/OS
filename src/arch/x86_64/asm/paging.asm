global g_page_table_l4
global g_page_table_l3
global g_page_table_l2
global g_page_table_pool

section .bss
align 4096
g_page_table_l4:
    resb 4096
g_page_table_l3:
    resb 4096
g_page_table_l2:
g_page_table_pool:
    resb 4096 * 4096