// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon_CameraShake.h"

UWeapon_CameraShake::UWeapon_CameraShake() {
	/*OscillationDuration = 0.25f;
	OscillationBlendInTime = 0.05f;
	OscillationBlendOutTime = 0.05f;

	RotOscillation.Pitch.Amplitude = FMath::RandRange(5.0f, 10.0f);
	RotOscillation.Pitch.Frequency = FMath::RandRange(25.0f, 35.0f);

	RotOscillation.Yaw.Amplitude = FMath::RandRange(5.0f, 10.0f);
	RotOscillation.Yaw.Frequency = FMath::RandRange(25.0f, 35.0f);

	LocOscillation.X.Amplitude = FMath::RandRange(5.0f, 10.0f);
	LocOscillation.X.Frequency = 1.0f;

	LocOscillation.Y.Amplitude = FMath::RandRange(5.0f, 10.0f);
	LocOscillation.Y.Frequency = 1.0f;*/

	OscillationDuration = 0.25f;
	OscillationBlendInTime = 0.1f;
	OscillationBlendOutTime = 0.2f;

	RotOscillation.Pitch.Amplitude = FMath::RandRange(5.0f, 10.0f);
	RotOscillation.Pitch.Frequency = 1.0f;

	RotOscillation.Yaw.Amplitude = 1.0f;
	RotOscillation.Yaw.Frequency = 1.0f;

}

void UWeapon_CameraShake::SetCameraShake(float _OscillationDuration, float _OscillationBlendInTime, float _OscillationBlendOutTime, float _Pitch_Amplitude, float _Pitch_Frequency, float _Yaw_Amplitude, float _Yaw_Frequency) {
	OscillationDuration = _OscillationDuration;
	OscillationBlendInTime = _OscillationBlendInTime;
	OscillationBlendOutTime = _OscillationBlendOutTime;

	RotOscillation.Pitch.Amplitude = FMath::RandRange(_Pitch_Amplitude, _Pitch_Amplitude*2.0f);
	RotOscillation.Pitch.Frequency = FMath::RandRange(_Pitch_Frequency, _Pitch_Frequency*1.45f);

	RotOscillation.Yaw.Amplitude = FMath::RandRange(_Yaw_Amplitude, _Yaw_Amplitude*2.0f);
	RotOscillation.Yaw.Frequency = FMath::RandRange(_Yaw_Frequency, _Yaw_Frequency*1.45f);
}