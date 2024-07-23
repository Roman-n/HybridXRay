#include "StdAfx.h"
#ifndef SHIPPING
#include "xrGameEditor.h"
extern void clean_game_globals();
extern void init_game_globals();
xrGameEditor::xrGameEditor()
{
    GameEditor = this;
}

xrGameEditor::~xrGameEditor() {}

void xrGameEditor::Create()
{
    init_game_globals();
}

void xrGameEditor::Destroy()
{
    clean_game_globals();
}
xrGameEditor GameEditorImpl;
#endif
