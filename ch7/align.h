enum { ALIGN_LEFT, ALIGN_RIGHT, ALIGN_CENTRE, ALIGN_JUSTIFIED };

int (*align(int (*)(void), int linelen, int maxrollover, int align))(void);
