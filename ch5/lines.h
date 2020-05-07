enum { LNS_OK = 0, LNS_ERROR = -1 };

int readlines(char **buffref, char ***linesref);
void writelines(char *lineptr[], int nlines);
void freelines(char *buff, char **lines);
