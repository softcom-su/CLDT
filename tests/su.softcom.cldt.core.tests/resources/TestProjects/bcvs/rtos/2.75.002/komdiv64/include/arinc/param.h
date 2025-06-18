#ifndef PARAM_H_
#define PARAM_H_
enum {
   PGP_CURRENT_WINDOW_START,
   PGP_CURRENT_WINDOW_LENGTH,
   PGP_REINCARNATION
};
int partGetParam(int name,void *res);
#endif /*PARAM_H_*/
