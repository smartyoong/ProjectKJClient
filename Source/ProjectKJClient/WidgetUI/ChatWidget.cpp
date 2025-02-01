// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetUI/ChatWidget.h"

// 추후 채팅 기능 만들때 UTF-8을 UTF-16으로 정상적으로 바꾸는 방법
//// UTF-8 인코딩된 문자열을 FString으로 변환하는 예시
//TArray<uint8> UTF8Data; // UTF-8 인코딩된 데이터를 포함하는 배열
//UTF8Data.Add(0); // 널 종료 문자 추가 (이건 없애도 될 수도 있음 내가 언리얼 코드 조작하면서 널문자 제거 안하도록 바꿈)
//
//// UTF-8 데이터를 FString으로 변환
//FString ChatMessage = UTF8_TO_TCHAR(reinterpret_cast<const char*>(UTF8Data.GetData()));
//
//// FString을 FText로 변환 (UI에 표시하기 위해)
//FText ChatMessageText = FText::FromString(ChatMessage);
//
//// UEditableTextBox에 텍스트 지정
//UEditableTextBox* MyEditableTextBox = ...; // UEditableTextBox 인스턴스를 얻는 방법은 상황에 따라 다름
//MyEditableTextBox->SetText(ChatMessageText);
