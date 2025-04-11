// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

class FToolBarBuilder;
class FMenuBuilder;

class FDataTableAutoGeneratorModule : public IModuleInterface
{
public:

	// 모듈이 로드될 때 최초 한번만 호출
	virtual void StartupModule() override;
	// 모듈이 언로드될 때 최초 한번만 호출 (에디터 종료, 플러그인 비활성화 등)
	virtual void ShutdownModule() override;
	
	/** This function will be bound to Command (by default it will bring up plugin window) */
	// 툴 바 버튼을 눌렀을 때 호출되는 함수 (UI창 띄우는 기능)
	void PluginButtonClicked();

private:
	void Init();
	void Shutdown();

	
private:
	// 메뉴 등록할 때 
	void RegisterMenus();

	// 플러그인 탭이 생성될 때 호출되는 함수. 커스텀 UI 탭 제작할
	TSharedRef<class SDockTab> OnSpawnPluginTab(const class FSpawnTabArgs& SpawnTabArgs);

	/* Customize MenuBar*/
	TSharedPtr<FExtender> MenuBarExtender;
	void AddMenuBarExtension(FMenuBarBuilder& MenuBarBuilder);
	void FillCustomMenu(FMenuBuilder& MenuBuilder);
	void FillSubMenu(FMenuBuilder& MenuBuilder);

	void OnClickGenerateTableMenu();

private:
	// 커맨드 목록 (단축키, 버튼 클릭 등 입력 이벤트를 명령으로 바인딩)
	TSharedPtr<class FUICommandList> PluginCommands;

	
};
