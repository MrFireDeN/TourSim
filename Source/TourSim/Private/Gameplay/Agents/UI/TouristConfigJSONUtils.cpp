// © 2026 Denis Sviridov. MIT License.


#include "Gameplay/Agents/UI/TouristConfigJSONUtils.h"

#include "DesktopPlatformModule.h"
#include "IDesktopPlatform.h"
#include "JsonObjectConverter.h"

bool UTouristConfigJSONUtils::SaveTouristConfigToFile(const FTouristConfigSharedFragment& Config)
{
	IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
	if (!DesktopPlatform)
	{
		return false;
	}
	
	const void* ParentWindowHandle = FSlateApplication::Get().FindBestParentWindowHandleForDialogs(nullptr);
	TArray<FString> OutFilenames;
	const FString StartPath = FPaths::ProjectSavedDir();
	
	bool bDialogResult = DesktopPlatform->SaveFileDialog(
	ParentWindowHandle,
	TEXT("Сохранить конфигурацию туристов"),
	StartPath,
	TEXT(""),
	TEXT("JSON файлы|*.json"),
	EFileDialogFlags::None,
	OutFilenames
	);
	
	if (!bDialogResult || OutFilenames.Num() == 0)
	{
		return false;
	}
	
	const FString& FilePath = OutFilenames[0];
	
	TSharedPtr<FJsonObject> JsonObject = FJsonObjectConverter::UStructToJsonObject(Config);
	if (!JsonObject)
	{
		return false;
	}
	
	FString OutputString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
	if (!FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer))
	{
		return false;
	}
	
	return FFileHelper::SaveStringToFile(OutputString, *FilePath, FFileHelper::EEncodingOptions::ForceUTF8WithoutBOM);
}

bool UTouristConfigJSONUtils::LoadTouristConfigFromFile(FTouristConfigSharedFragment& OutConfit)
{
	IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
	if (!DesktopPlatform)
	{
		return false;
	}
	
	const void* ParentWindowHandle = FSlateApplication::Get().FindBestParentWindowHandleForDialogs(nullptr);
	TArray<FString> OutFilenames;
	const FString StartPath = FPaths::ProjectSavedDir();
	
	bool bDialogResult = DesktopPlatform->OpenFileDialog(
		ParentWindowHandle,
		TEXT("Загрузить конфигурацию туристов"),
		StartPath,
		TEXT(""),
		TEXT("JSON файлы|*.json"),
		EFileDialogFlags::None,
		OutFilenames
	);
	
	if (!bDialogResult || OutFilenames.Num() == 0)
		return false;

	const FString& FilePath = OutFilenames[0];
	
	FString JsonString;
	if (!FFileHelper::LoadFileToString(JsonString, *FilePath))
	{
		return false;
	}
	
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);
	if (!FJsonSerializer::Deserialize(Reader, JsonObject) || !JsonObject)
	{
		return false;
	}
	
	return FJsonObjectConverter::JsonObjectToUStruct(JsonObject.ToSharedRef(), &OutConfit);
}
