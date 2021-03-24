// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(EndBoy_Cpp, Log, All);
#define EBLOG_CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))
#define EBLOG_S(Verbosity) UE_LOG(EndBoy_Cpp, Verbosity, TEXT("%s"), *EBLOG_CALLINFO)
#define EBLOG(Verbosity, Format, ...) UE_LOG(EndBoy_Cpp, Verbosity, TEXT("%s %s"), *EBLOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__));