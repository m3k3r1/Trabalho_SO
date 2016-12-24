#ifndef _THD_MGMT_H
#define _THD_MGMT_H

#include <pthread.h>
#include "base.h"
#include "usr_mgmt.h"
#include "usr_ui.h"

bool END_THD;

void* init_fld(void* th);

#endif
