#include "Application.h"

#include <QCloseEvent>
#include <QDesktopWidget>
#include <QDirIterator>
#include <QFile>
#include <QFileInfo>
#include <QMessageBox>
#include <QTimer>
#include <QSettings>



namespace QtOgre
{

	Application::Application(int& argc, char** argv)
	:QApplication(argc, argv)
	,mInternalOgreLog(0)
	,mInternalOgreLogManager(0)
	,mActiveRenderSystem(0)
	,mOpenGLRenderSystem(0)
	,mDirect3D9RenderSystem(0)
	,mRoot(0)
	,mFrameCounter(0)
	,mAutoUpdateTimer(0)
	,mSettings(0)
	,mAutoUpdateEnabled(true)
	,mIsInitialised(false)
	{




		//Sanity check for config files


		mAutoUpdateTimer = new QTimer;
		QObject::connect(mAutoUpdateTimer, SIGNAL(timeout()), this, SLOT(update()));
		//On the test system, a value of one here gives a high frame rate and still allows
		//event processing to take place. A value of 0 doubles the frame rate but the mouse
		//becomes jumpy. This property is configerable via setAutoUpdateInterval().
		mAutoUpdateTimer->setInterval(1);

		//Load the settings file. If it doesn't exist it is created.
		mSettings = new QSettings("settings.ini", QSettings::IniFormat);

		//mOgreWidget = new EventHandlingOgreWidget(0, 0);


		//Create the Ogre::Root object.
		qDebug("Creating Ogre::Root object...");
		try
		{
#ifdef QT_DEBUG
			mRoot = new Ogre::Root("plugins_d.cfg");
#else
			mRoot = new Ogre::Root("plugins.cfg");
#endif
			qDebug("Ogre::Root created successfully.");
		}
		catch(Ogre::Exception& e)
		{
			QString error
				(
				"Failed to create the Ogre::Root object. This is a fatal error and the "
				"application will now exit. There are a few known reasons why this can occur:\n\n"
				"    1) Ensure your plugins.cfg has the correct path to the plugins.\n"
				"    2) In plugins.cfg, use unix style directorary serperators. I.e '/' rather than '\\'.\n"
				"    3) If your plugins.cfg is trying to load the Direct3D plugin, make sure you have DirectX installed on your machine.\n\n"
				"The message returned by Ogre was:\n\n"
				);
			error += QString::fromStdString(e.getFullDescription().c_str());

			qCritical(error.toAscii());
			showErrorMessageBox(error);

			//Not much else we can do here...
			std::exit(1);
		}

		//Load the render system plugins. We do that here so that we know what
		//render systems are available by the time we show the settings dialog.
		//Note that the render system is not initialised until the user selects one.
		mOpenGLRenderSystem = mRoot->getRenderSystemByName("OpenGL Rendering Subsystem");
		mDirect3D9RenderSystem = mRoot->getRenderSystemByName("Direct3D9 Rendering Subsystem");
		if(!(mOpenGLRenderSystem || mDirect3D9RenderSystem))
		{
			qCritical("No rendering subsystems found");
			showErrorMessageBox("No rendering subsystems found. Please ensure that your 'plugins.cfg' (or 'plugins_d.cfg') is correct and can be found by the executable.");
		}



	}

	Application::~Application()
	{
		if (mRoot)
		{
            delete mRoot;
			mRoot = 0;
		}

		//We delete the OgreWidget last because it
		//owns the LogManager (through Qt's mechanism).
	}

	////////////////////////////////////////////////////////////////////////////////
	/// \return whether the OpenGL render system is available
	////////////////////////////////////////////////////////////////////////////////
	bool Application::isOpenGLAvailable(void) const
	{
		return mOpenGLRenderSystem != 0;
	}

	////////////////////////////////////////////////////////////////////////////////
	/// \return whether the Direct3D9 render system is available.
	////////////////////////////////////////////////////////////////////////////////
	bool Application::isDirect3D9Available(void) const
	{
		return mDirect3D9RenderSystem != 0;
	}

	////////////////////////////////////////////////////////////////////////////////
	/// \return the number of frames rendered since the application started.
	////////////////////////////////////////////////////////////////////////////////
	unsigned int Application::frameCount(void) const
	{
		return mFrameCounter;
	}

	////////////////////////////////////////////////////////////////////////////////
	///This function is an implementation detail, and should not really be exposed.
	///It return the log which the QtOgre framework uses for its messages, whereas
	///users are expected to instead create their own log with createLog(). The reason
	///it is exposed is that the Qt debugging system (qtMessageHandler()) also redirects
	///to this log, and that cannot be made a member function.
	/// \return the log used by the QtOgre framework.
	////////////////////////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////////////////////////
	/// \return a pointer to the applications main window.
	////////////////////////////////////////////////////////////////////////////////



	void Application::initialise(void)
	{


		initialiseOgre();

		Ogre::NameValuePairList ogreWindowParams;
		//ogreWindowParams["FSAA"] = "8";


		//Set up resource paths. This can't be done until the OgreWidget
		//is initialised, because we need the GPUProgramManager.
		if(QFile::exists("resources.cfg"))
		{
			loadResourcePathsFromConfigFile("resources.cfg");
			Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
		}


		if(mAutoUpdateEnabled)
		{
			mAutoUpdateTimer->start();
		}

		mIsInitialised = true;
	}

	void Application::update(void)
	{
		++mFrameCounter;
	}

	void Application::shutdown(void)
	{
		mAutoUpdateTimer->stop();
		this->exit(0);
	}


	void Application::initialiseOgre(void)
	{
		Ogre::Root::getSingletonPtr()->setRenderSystem(mOpenGLRenderSystem);
		Ogre::Root::getSingletonPtr()->initialise(false);
	}

	////////////////////////////////////////////////////////////////////////////////
	/// Centers a widget inside its parent, or inside the desktop if no
	/// parent is provided. This requires the widget to be free to move.
	/// If not (e.g. its in a layout) then the behaviour is undefined
	/// \param widgetToCenter the widget to centre
	/// \param parent the parent of the widget
	////////////////////////////////////////////////////////////////////////////////	
	void Application::centerWidget(QWidget* widgetToCenter, QWidget* parent)
	{
		QRect parentGeometry;
		if(parent != 0)
		{
			parentGeometry = parent->frameGeometry();
		}
		else
		{
			parentGeometry = desktop()->availableGeometry();
		}

		int xOffset = (parentGeometry.width() - widgetToCenter->frameGeometry().width()) / 2;
		int yOffset = (parentGeometry.height() - widgetToCenter->frameGeometry().height()) / 2;
		widgetToCenter->move(parentGeometry.x() + xOffset, parentGeometry.y() + yOffset);
	}

	////////////////////////////////////////////////////////////////////////////////
	/// \param text the text to display
	////////////////////////////////////////////////////////////////////////////////
	void Application::showInfoMessageBox(const QString& text)
	{
		QMessageBox msgBox;
		msgBox.setWindowTitle("Information");
		msgBox.setIconPixmap(QPixmap(":/images/icons/dialog-information.svg"));
		msgBox.setStandardButtons(QMessageBox::Ok);
		msgBox.setText(text);
		msgBox.exec();
	}

	////////////////////////////////////////////////////////////////////////////////
	/// \param text the text to display
	////////////////////////////////////////////////////////////////////////////////
	void Application::showWarningMessageBox(const QString& text)
	{
		QMessageBox msgBox;
		msgBox.setWindowTitle("Warning");
		msgBox.setIconPixmap(QPixmap(":/images/icons/dialog-warning.svg"));
		msgBox.setStandardButtons(QMessageBox::Ok);
		msgBox.setText(text);
		msgBox.exec();
	}

	////////////////////////////////////////////////////////////////////////////////
	/// \param text the text to display
	////////////////////////////////////////////////////////////////////////////////
	void Application::showErrorMessageBox(const QString& text)
	{
		QMessageBox msgBox;
		msgBox.setWindowTitle("Error");
		msgBox.setIconPixmap(QPixmap(":/images/icons/dialog-error.svg"));
		msgBox.setStandardButtons(QMessageBox::Ok);
		msgBox.setText(text);
		msgBox.exec();
	}

	

	////////////////////////////////////////////////////////////////////////////////
	/// \return a pointer to the application settings
	////////////////////////////////////////////////////////////////////////////////
	QSettings* Application::settings(void) const
	{
		return mSettings;
	}

	/**
	* Sets the period between sucessive updates.
	* \param intervalInMilliseconds the period between sucessive updates
	*/
	void Application::setAutoUpdateInterval(int intervalInMilliseconds)
	{
		mAutoUpdateTimer->setInterval(intervalInMilliseconds);
	}


	void Application::setAutoUpdateEnabled(bool autoUpdateEnabled)
	{
		mAutoUpdateEnabled = autoUpdateEnabled;

		//Only call start if the app is initialised, otherwise
		//the update() function might be using null pointers.
		if(mAutoUpdateEnabled && mIsInitialised)
		{
			mAutoUpdateTimer->start();
		}
		else
		{
			mAutoUpdateTimer->stop();
		}
	}

	void Application::warnAboutIgnoredConfigFile(const QString& filename)
	{
		QString message;
		message += "The file \'" + filename + "\' has been found in the applications working directory.";
		message += "\n\n";
		message += "The '.cfg' extension implies the file may usually be used by Ogre and/or the ExampleApplication framework. ";
		message += "However, the QtOgre framework currently only supports the 'plugins.cfg', 'plugins_d.cfg', and 'resources.cfg' files. ";
		message += "The file will be ignored by the QtOgre framework, and your application may not behave as expected. ";
		message += "\n\n";
		message += "If the file is being used by your application code, or by one of the plugins which you are loading, then you can suppress ";
		message += "this warning dialog box by passing the appropriate flags to the Application constructor. Please consult the documentation.";
		showWarningMessageBox(message);
	}

	//This function is based on code from the Ogre ExampleApplication.
	//It is not constrained by the Ogre licence (free for any use).
	void Application::loadResourcePathsFromConfigFile(const QString& filename)
    {
        // Load resource paths from config file
		Ogre::ConfigFile cf;
		cf.load(filename.toStdString());

        // Go through all sections & settings in the file
        Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

        Ogre::String secName, typeName, archName;
        while (seci.hasMoreElements())
        {
            secName = seci.peekNextKey();
            Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
            Ogre::ConfigFile::SettingsMultiMap::iterator i;
            for (i = settings->begin(); i != settings->end(); ++i)
            {
                typeName = i->first;
                archName = i->second;
                Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
                    archName, typeName, secName);
            }
        }
    }
}
