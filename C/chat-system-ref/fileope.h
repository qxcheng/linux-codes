#ifndef FILE_OPE_H
#define FILE_OPE_H

#include "usrLink.h"

int openFile(const char *path, int flags);
void saveUsrInfoToFile(const UsrList *list);
void getUsrInfoFromFile(UsrList *list);

#endif
