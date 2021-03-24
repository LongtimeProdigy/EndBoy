// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EndBoy_Cpp.h"
#include "Camera/CameraShake.h"
#include "Weapon_CameraShake.generated.h"

/**
 * 
 */
UCLASS()
class ENDBOY_CPP_API UWeapon_CameraShake : public UCameraShake
{
	GENERATED_BODY()

	UWeapon_CameraShake();

public:
	void SetCameraShake(float _OscillationDuration, float _OscillationBlendInTime, float _OscillationBlendOutTime, float _Pitch_Amplitude, float _Pitch_Frequency, float _Yaw_Amplitude, float _Yaw_Frequency);
	
};
