﻿#include "stdafx.h"
#include "physics_shell_animated.h"

#include "physicsshell.h"

#include "../xrRender/Public/Kinematics.h"

physics_shell_animated::physics_shell_animated(CGameObject* O, bool _update_velocity): update_velocity(_update_velocity)
{
    create_shell(O);
}

bool physics_shell_animated::update(const Fmatrix& xrorm)
{
    VERIFY(physics_shell);

    if (update_velocity)
        physics_shell->AnimToVelocityState(Device->fTimeDelta, default_l_limit * 10, default_w_limit * 10);

    physics_shell->mXFORM.set(xrorm);
    physics_shell->PKinematics()->CalculateBones();
    physics_shell->ToAnimBonesPositions();
    return true;
}

physics_shell_animated::~physics_shell_animated()
{
    destroy_physics_shell(physics_shell);
}

void physics_shell_animated::create_shell(CGameObject* O)
{
    physics_shell = P_build_Shell(O, true, (BONE_P_MAP*)0, true);
    physics_shell->ToAnimBonesPositions();
    physics_shell->DisableCollision();
    physics_shell->SetAnimated(true);
}
