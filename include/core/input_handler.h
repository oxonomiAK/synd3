#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include "tui/tui_manager.h"
#include "core/process_kill.h"

int handleMainInput(ptParams *pTableParams, d_arr *processes, TUIManager wins);
void handle_winch(TUIManager wins);
int handleAboutInput(TUIManager wins);
int handlePopupInput(TUIManager wins, int selectedProcess, d_arr *processes);
int handleExtraOptInput(TUIManager wins, extOpt *extraOpt);

#endif