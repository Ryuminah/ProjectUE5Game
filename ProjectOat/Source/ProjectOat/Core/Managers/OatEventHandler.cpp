// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Managers/OatEventHandler.h"

void AOatEventHandler::Initialize(UGameInstance* GameInstance)
{

}

void AOatEventHandler::AddStageSectionChangedDelegate(UObject* Object, void (UObject::*Function)(int, EStageSectionState))
{
	OnStageSectionChanged.AddUObject(Object, Function);
}
