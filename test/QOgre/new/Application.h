#pragma once

#include <OGRE/Ogre.h>

#include <QApplication>

class QSettings;

namespace QtOgre
{
	class Application : public QApplication
	{
		Q_OBJECT

	public:
		/// Creates an instance of the Application class.
		Application(int & argc, char ** argv);
		/// Destroys an instance of the Application class
		~Application();

		///\name Getters
		//@{
		/// The total number of frames rendered
		unsigned int frameCount(void) const;
		/// Get the OGRE RenderWindow for adding viewports   
		Ogre::RenderWindow* ogreRenderWindow(void) const;
		/// Get the main window widget
		QWidget* mainWidget(void) const;
		/// Access the application settings
		QSettings* settings(void) const;

		//@}

		///\name Setters
		//@{
		/// Sets the period between sucessive updates.
		void setAutoUpdateInterval(int intervalInMilliseconds);
		/// Controls whether QtOgre periodically calls update().
		void setAutoUpdateEnabled(bool autoUpdateEnabled);
		//@}

		///\name Testers
		//@{
		/// Determine whether the OpenGL render system is available
		bool isOpenGLAvailable(void) const;
		/// Determine whether the Direct3D9 render system is available.
		bool isDirect3D9Available(void) const;
		//@}
		

		
		//Static functions
		/// Start the main event loop.
		/// Utility function to center a widget.
		static void centerWidget(QWidget* widgetToCenter, QWidget* parent = 0);

		/// Shows a message box with an 'Info' icon and 'Information' in the title bar.
		static void showInfoMessageBox(const QString& text);
		/// Shows a message box with a 'Warning' icon and 'Warning' in the title bar.
		static void showWarningMessageBox(const QString& text);
		/// Shows a message box with an 'Error' icon and 'Error' in the title bar.
		static void showErrorMessageBox(const QString& text);

	public slots:
		void initialise(void);
		void shutdown(void);
		void update(void);

	private:
		//Private functions
		void initialiseOgre(void);
		///Implemented from Ogre::LogListener to redirect logging 
		void loadResourcePathsFromConfigFile(const QString& filename);

		//Logging
		Ogre::Log* mInternalOgreLog;
		Ogre::LogManager* mInternalOgreLogManager;

		//Ogre Stuff
		Ogre::RenderSystem* mActiveRenderSystem;
		Ogre::RenderSystem* mOpenGLRenderSystem;
		Ogre::RenderSystem* mDirect3D9RenderSystem;
		Ogre::Root* mRoot;

		//Config warnings
		void warnAboutIgnoredConfigFile(const QString& filename);

		//Misc
		unsigned int mFrameCounter;
		QTimer* mAutoUpdateTimer;
		QSettings* mSettings;
		bool mAutoUpdateEnabled;
		bool mIsInitialised;
	};
}
