#include "MyUserWidget.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/Image.h"
#include "MediaPlaylist.h"

void UMyUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	MediaTexture = NewObject<UMediaTexture>();
	MediaPlayer = NewObject<UMediaPlayer>();
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
	MediaTexture->SetMediaPlayer(MediaPlayer);
	UMediaPlaylist* Playlist = NewObject<UMediaPlaylist>();
	if (MediaSource != nullptr)
	{
		if (!MediaPlayer->OpenSource(MediaSource))
		{
			UE_LOG(LogTemp, Error, TEXT("Cannot Open Media Source"));
		}
		else
		{
			MediaPlayer->SetLooping(true);
			Playlist->Add(MediaSource);
			MediaPlayer->OpenPlaylist(Playlist);
			MediaPlayer->Play();
		}
	}
	if (MainImage != nullptr)
	{
		FSlateBrush Brush;
		Brush.SetResourceObject(MediaTexture);
		MainImage->SetBrush(Brush);
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

