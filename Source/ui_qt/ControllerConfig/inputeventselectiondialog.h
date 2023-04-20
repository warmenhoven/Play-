#pragma once

#include <QDialog>
#include <QKeyEvent>
#include <chrono>
#include <thread>
#include "input/InputBindingManager.h"
#include "InputProviderQtKey.h"
#include "InputProviderQtMouse.h"

namespace Ui
{
	class InputEventSelectionDialog;
}

class InputEventSelectionDialog : public QDialog
{
	Q_OBJECT

public:
	explicit InputEventSelectionDialog(QWidget* parent = nullptr);
	~InputEventSelectionDialog();

	void Setup(const char*, CInputBindingManager*, CInputProviderQtKey*, CInputProviderQtMouse*, uint32, PS2::CControllerInfo::BUTTON);

protected:
	void keyPressEvent(QKeyEvent*) Q_DECL_OVERRIDE;
	void keyReleaseEvent(QKeyEvent*) Q_DECL_OVERRIDE;
	void mousePressEvent(QMouseEvent*) Q_DECL_OVERRIDE;
	void mouseReleaseEvent(QMouseEvent*) Q_DECL_OVERRIDE;

private:
	enum class STATE
	{
		WAITING,
		SELECTED,
		SIMULATEDAXIS_WAITING,
		SIMULATEDAXIS_SELECTED,
	};

	void onInputEvent(const BINDINGTARGET&, uint32);

	Ui::InputEventSelectionDialog* ui = nullptr;
	QTimer* m_countdownTimer = nullptr;
	uint32 m_countdownRemain = 0;

	uint32 m_padIndex = 0;
	PS2::CControllerInfo::BUTTON m_button;
	QString m_buttonName;
	CInputBindingManager* m_inputManager = nullptr;
	CInputProviderQtKey* m_qtKeyInputProvider = nullptr;
	CInputProviderQtMouse* m_qtMouseInputProvider = nullptr;

	STATE m_state = STATE::WAITING;
	BINDINGTARGET m_selectedTarget;
	BINDINGTARGET m_selectedTargetSimulatedAxis;
	CInputBindingManager::BINDINGTYPE m_bindingType = CInputBindingManager::BINDING_UNBOUND;
	uint32 m_bindingValue = 0;

	QString m_bindingText = QString("Select new binding for\n%1");
	QString m_nextbindingText = QString("Select axis negative binding for\n%1");
	QString m_countingText = QString("Press & Hold Button for %1 Seconds to assign key");
	QString m_selectedkeyText = QString("Selected Key: %1");

private slots:
	void handleStartCountdown(QString);
	void handleCancelCountdown();
	void updateCountdown();
	void confirmBinding();

Q_SIGNALS:
	void startCountdown(QString);
	void cancelCountdown();
	void countdownComplete();
};
