// Copyright Epic Games, Inc. All Rights Reserved.

#include "DataTableAutoGenerator.h"
#include "DataTableAutoGeneratorStyle.h"
#include "DataTableAutoGeneratorCommands.h"
#include "FDTLoader.h"
#include "LevelEditor.h"
#include "SDTGeneratorWidget.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"

static const FName DataTableAutoGeneratorTabName("DTGenerator");

#define LOCTEXT_NAMESPACE "FDataTableAutoGeneratorModule"

void FDataTableAutoGeneratorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	// 스타일 셋을 초기화
	// 아이콘 경로같은 것을 설정해둔다
	FDataTableAutoGeneratorStyle::Initialize();
	FDataTableAutoGeneratorStyle::ReloadTextures();

	// UI 명령어 등록
	FDataTableAutoGeneratorCommands::Register();
	// 버튼 클릭 등을 처리할 수 있게 FUICommandList를 생성함)	        
	PluginCommands = MakeShareable(new FUICommandList);

	// 특정 명령어와 클릭 이벤트 연결                             
	PluginCommands->MapAction(
	                          FDataTableAutoGeneratorCommands::Get().OpenPluginWindow,
	                          FExecuteAction::CreateRaw(this, &FDataTableAutoGeneratorModule::PluginButtonClicked),
	                          FCanExecuteAction());

	// 에디터 메뉴에 항목 추가
	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FDataTableAutoGeneratorModule::RegisterMenus));

	// 탭(도킹UI)를 등록해 플러그인 창을 띄울 준비
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(DataTableAutoGeneratorTabName, FOnSpawnTab::CreateRaw(this, &FDataTableAutoGeneratorModule::OnSpawnPluginTab))
	                        .SetDisplayName(LOCTEXT("FDataTableAutoGeneratorTabTitle", "DTGenerator"))
	                        .SetMenuType(ETabSpawnerMenuType::Hidden);


	// 기타 클래스들을 생성하는 함수
	Init();
}

void FDataTableAutoGeneratorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);
	UToolMenus::UnregisterOwner(this);
	FDataTableAutoGeneratorStyle::Shutdown();
	FDataTableAutoGeneratorCommands::Unregister();
	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(DataTableAutoGeneratorTabName);

	// Shutdown MenuBarExtender
	MenuBarExtender.Reset();
	Shutdown();
}

TSharedRef<SDockTab> FDataTableAutoGeneratorModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	// 창 열렸을 떄 문구
	FText WidgetText = FText::Format(
	                                 LOCTEXT("WindowWidgetText", "Add code to {0} in {1} to override this window's contents"),
	                                 FText::FromString(TEXT("FDataTableAutoGeneratorModule::OnSpawnPluginTab")),
	                                 FText::FromString(TEXT("DataTableAutoGenerator.cpp"))
	                                );

	// 보여줄 탭 콘텐츠를 구성하는 함수
	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			// Put your tab content here!
			SNew(SBox)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(WidgetText)
			]
		];
}

// 등록한 탭을 에디터 창에 실제로 띄움 (메뉴나 버튼에서 호출되는 함수)
void FDataTableAutoGeneratorModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(DataTableAutoGeneratorTabName);
}

void FDataTableAutoGeneratorModule::Init()
{

}

void FDataTableAutoGeneratorModule::Shutdown()
{

}

// 메뉴 및 툴바에 커맨드 등록
void FDataTableAutoGeneratorModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	// 나중에 해제하기 위하여 등록한 주체를 저장
	FToolMenuOwnerScoped OwnerScoped(this);

//	{
//		// "LevelEditor.MainMenu.Window"에 플러그인용 버튼을 추가 
//		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
//		{
//			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
//			Section.AddMenuEntryWithCommandList(FDataTableAutoGeneratorCommands::Get().OpenPluginWindow, PluginCommands);
//		}
//	}
//	{
//		// "LevelEditor.LevelEditorToolBar"에 플러그인용 버튼을 추가
//		// 커맨드 리스트(PluginCommands)를 버튼에 연결하므로써 버튼이 눌렸을ㄸㅐ  FDataTableAutoGeneratorModule::PluginButtonClicked()가 호출되는 것..
//		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
//		{
//			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Settings");
//			{
//				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FDataTableAutoGeneratorCommands::Get().OpenPluginWindow));
//				Entry.SetCommandList(PluginCommands);
//			}
//		}
//	}

	/* Extend Main MenuBar*/
	// 상단바 제작을 위하여 LevelEditorModule 불러옴 (크래시 방지를 위하여 Checked)
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");

	// FExtender : 메뉴, 툴 바에 새 항목을 확장하도록 해주는 객체 (여기다 만들고 싶은 메뉴 정보를 채워 넣으면 됨)
	MenuBarExtender = MakeShareable(new FExtender);
	// Help 메뉴 뒤에 삽입
	MenuBarExtender->AddMenuBarExtension("Help", EExtensionHook::After, PluginCommands,
	                                     // AddMenuBarExtension 콜백에서 새 탭을 눌렀을 때 어떤 메뉴들이 뜨는지 처리
	                                     FMenuBarExtensionDelegate::CreateRaw(this, &FDataTableAutoGeneratorModule::AddMenuBarExtension));

	// 작성한 메뉴 확장자를 에디터에 등록
	LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuBarExtender);

}

/* Customize MenuBar */
void FDataTableAutoGeneratorModule::AddMenuBarExtension(FMenuBarBuilder& MenuBarBuilder)
{
	// Tab이름 (로컬라이징 용 키 , 실제 라벨 텍스트)
	MenuBarBuilder.AddPullDownMenu(LOCTEXT("Custom", "Custom"),
	                               LOCTEXT("Custom_ToolTip", " Custom Plugins"),                                      // 툴팁
	                               FNewMenuDelegate::CreateRaw(this, &FDataTableAutoGeneratorModule::FillCustomMenu), // 눌렀을 때 열리는 메뉴 구성의 콜백
	                               "DTGenerator");                                                                    // 내부적으로 사용될 탭의 고유 D                                                                        
}

void FDataTableAutoGeneratorModule::FillCustomMenu(FMenuBuilder& MenuBuilder)
{
	// 드롭다운으로 생긴 버튼의 콜백 -> Window를 열게끔 되어있다
	//MenuBuilder.AddMenuEntry(FDataTableAutoGeneratorCommands::Get().OpenPluginWindow);

	// 드롭다운 한번 더
	// LOCTEXT(번역 키 값, 실제 이름)
	MenuBuilder.AddSubMenu(LOCTEXT("DTGenerator", "DTGenerator"),
	                       LOCTEXT("DataTableMenu_ToolTip", " Loading Spreadsheet and Automatically Generating DataTable Assets"),
	                       FNewMenuDelegate::CreateRaw(this, &FDataTableAutoGeneratorModule::FillSubMenu), false, FSlateIcon());

}

void FDataTableAutoGeneratorModule::FillSubMenu(FMenuBuilder& MenuBuilder)
{
	{
//		MenuBuilder.AddMenuEntry(LOCTEXT("GenerateTable","GenerateTable"),
//			LOCTEXT("GenerateTable_ToolTip", "Generate a DataTable from Spreadsheet"),FSlateIcon(),
//			FUIAction(FExecuteAction::CreateRaw(this, TODO:: AddGenerateCallbackFunc)));
		// 나중에 콜백추가                                              
		MenuBuilder.AddMenuEntry(LOCTEXT("GenerateTable", "GenerateTable"),
		                         LOCTEXT("GenerateTable_ToolTip", "Generate DataTable from Spreadsheet"),
		                         FSlateIcon(),
		                         FUIAction(FExecuteAction::CreateRaw(this, &FDataTableAutoGeneratorModule::OnClickGenerateTableMenu)));

	}

	{
		MenuBuilder.AddMenuEntry(LOCTEXT("CheckValid", "CheckValid"),
		                         LOCTEXT("CheckValid_ToolTip", "Check Validate TableData"),
		                         FSlateIcon(), FUIAction());
	}
}

void FDataTableAutoGeneratorModule::OnClickGenerateTableMenu()
{
	TSharedRef<SWindow> Window = SNew(SWindow)
		.Title(LOCTEXT("Settings", "Generate Settings"))
		.SupportsMinimize(false).SupportsMaximize(false)
		.ClientSize(FVector2D(400, 300))
		[
			SNew(SDTGeneratorWidget)
		];

	FSlateApplication::Get().AddWindow(Window);
}
#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FDataTableAutoGeneratorModule, DataTableAutoGenerator)
