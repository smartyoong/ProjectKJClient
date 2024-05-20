#include "MyUserWidget.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"

void UMyUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	// 미디어 텍스처 (화면 캡처용)
	MediaTexture = NewObject<UMediaTexture>();
	MediaPlayer = NewObject<UMediaPlayer>();
	// 버튼 설정
	if(LoginButton != nullptr)
		LoginButton->OnClicked.AddDynamic(this, &UMyUserWidget::OnLoginButtonClicked);
	if (RegistButton != nullptr)
		RegistButton->OnClicked.AddDynamic(this, &UMyUserWidget::OnRegistButtonClicked);
	if (IDTextBox != nullptr)
	{
		IDTextBox->OnTextChanged.AddDynamic(this, &UMyUserWidget::OnIDTextChanged);
	}
	if (PasswordTextBox != nullptr)
	{
		PasswordTextBox->OnTextChanged.AddDynamic(this, &UMyUserWidget::OnPasswordTextChanged);
	}
	// 미디어 플레이어와 텍스처 연결
	MediaTexture->SetMediaPlayer(MediaPlayer);
	MediaTexture->UpdateResource();
	if (MainImage != nullptr)
	{
		FSlateBrush Brush;
		Brush.SetResourceObject(MediaTexture);
		MainImage->SetBrush(Brush);
	}

	// 미디어 플레이어와 플레이리스트 연결
	Playlist = NewObject<UMediaPlaylist>();
	if (MediaSource != nullptr)
	{
		MediaPlayer->SetLooping(true);
		MediaPlayer->SetNativeVolume(1.0f);
		Playlist->Add(MediaSource);
		MediaPlayer->OpenPlaylist(Playlist);
		MediaPlayer->Play();
		// 미디어 플레이어와 사운드 연결
		if (SoundToPlay != nullptr)
			UGameplayStatics::PlaySound2D(this, SoundToPlay);
	}
}

void UMyUserWidget::OnLoginButtonClicked()
{
	// 패킷 송수신하도록 작업하고 여기에 추가하자
	UE_LOG(LogTemp, Warning, TEXT("Login Button Clicked"));
}

void UMyUserWidget::OnRegistButtonClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("Regist Button Clicked"));
	// 나중에 회원가입도 만들자
}

void UMyUserWidget::OnIDTextChanged(const FText& text)
{
	if (IsFirstTry)
	{
		IDTextBox->SetText(FText::GetEmpty());
		IsFirstTry = false;
	}
	ID = text.ToString();
}

void UMyUserWidget::OnPasswordTextChanged(const FText& text)
{
	if (IsFirstPWTry)
	{
		PasswordTextBox->SetText(FText::GetEmpty());
		IsFirstPWTry = false;
		PasswordTextBox->SetIsPassword(true);
	}
	Password = text.ToString();
}

