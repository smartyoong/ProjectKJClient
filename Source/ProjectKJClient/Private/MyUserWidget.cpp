#include "MyUserWidget.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"
#include "LoginPacketManager.h"
#include "ProjectKJClient/MainGameInstance.h"
#include "LoginResultWidget.h"
#include "RegistAccountUserWidget.h"

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
		IDTextBox->OnTextCommitted.AddDynamic(this, &UMyUserWidget::OnIDTextCommitted);
		IDTextBox->SetSelectAllTextWhenFocused(true);
	}
	if (PasswordTextBox != nullptr)
	{
		PasswordTextBox->OnTextCommitted.AddDynamic(this, &UMyUserWidget::OnPasswordTextCommitted);
		PasswordTextBox->OnTextChanged.AddDynamic(this, &UMyUserWidget::OnPasswordChanged);
		PasswordTextBox->SetSelectAllTextWhenFocused(true);
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
	FLoginRequestPacket LoginPacket;
	LoginPacket.AccountID = ID;
	LoginPacket.Password = Password;
	LoginPacketListID PacketID = LoginPacketListID::LOGIN_REQUEST;
	GetWorld()->GetGameInstance<UMainGameInstance>()->SendPacketToLoginServer<FLoginRequestPacket>(PacketID,LoginPacket);
}

void UMyUserWidget::OnRegistButtonClicked()
{
	ShowRegistAccountWidget();
}

void UMyUserWidget::OnIDTextCommitted(const FText& text, ETextCommit::Type TextType)
{
	ID = text.ToString();
}

void UMyUserWidget::OnPasswordTextCommitted(const FText& text, ETextCommit::Type TextType)
{
	Password = text.ToString();
}

void UMyUserWidget::OnPasswordChanged(const FText& text)
{
	PasswordTextBox->SetIsPassword(true);
}

void UMyUserWidget::ShowLoginResultWidget(int Mode)
{
	if(LoginResultWidgetClass == nullptr)
		return;
	AsyncTask(ENamedThreads::GameThread, [this, Mode]()
		{
			LoginResultWidget = CreateWidget<ULoginResultWidget>(GetWorld(), LoginResultWidgetClass);
			if (LoginResultWidget != nullptr)
			{
				switch (Mode)
				{
				case 0:
					LoginResultWidget->SetNoAccount();
					break;
				case 1:
					LoginResultWidget->SetPasswordFail();
					break;
				case 2:
					LoginResultWidget->SetLoginSuceess();
					break;
				default:
					LoginResultWidget->SetTryLoginLater();
					break;
				}
				LoginResultWidget->AddToViewport();
			}
		});
}

void UMyUserWidget::ShowRegistAccountWidget()
{
	AsyncTask(ENamedThreads::GameThread, [this]()
		{
			RegistAccountWidget = CreateWidget<URegistAccountUserWidget>(GetWorld(), RegistAccountWidgetClass);
			if (RegistAccountWidget != nullptr)
			{
				RegistAccountWidget->SetParentWidget(this);
				RegistAccountWidget->AddToViewport();
			}
			IDTextBox->SetVisibility(ESlateVisibility::Hidden);
			PasswordTextBox->SetVisibility(ESlateVisibility::Hidden);
			LoginButton->SetVisibility(ESlateVisibility::Hidden);
			RegistButton->SetVisibility(ESlateVisibility::Hidden);
		});
}

void UMyUserWidget::ShowWidgetItems()
{
	AsyncTask(ENamedThreads::GameThread, [this]()
		{
			IDTextBox->SetVisibility(ESlateVisibility::Visible);
			PasswordTextBox->SetVisibility(ESlateVisibility::Visible);
			LoginButton->SetVisibility(ESlateVisibility::Visible);
			RegistButton->SetVisibility(ESlateVisibility::Visible);
		});
}

void UMyUserWidget::ShowRegistSuccessPopUp()
{
	if(LoginResultWidgetClass == nullptr)
		return;
	AsyncTask(ENamedThreads::GameThread, [this]()
	{
		LoginResultWidget = CreateWidget<ULoginResultWidget>(GetWorld(), LoginResultWidgetClass);
		if(LoginResultWidget != nullptr)
		{
			LoginResultWidget->SetRegistSuccess();
			LoginResultWidget->AddToViewport();
		}
	});
}

URegistAccountUserWidget* UMyUserWidget::GetRegistAccountWidget()
{
	return RegistAccountWidget;
}

