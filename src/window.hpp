#pragma once

#include <QtWidgets/QWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QLabel>
#include <QtGui/QPalette>

class VoleurWindow: public QWidget {
	public:
		VoleurWindow();	
	private:
		QLineEdit* licensorInput;
		QPushButton* generateButton;
		QLabel* statusText;

		void handleGenerateButton();
		void showError(const QString& str);
		void showSuccess(const QString& str);
};