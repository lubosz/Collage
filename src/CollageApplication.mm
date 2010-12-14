/*
 * CollageApplication.cpp
 *
 *  Created on: Nov 12, 2010
 *  Author: bmonkey
 *  Copyright 2010 The Collage Project
 */

#import <Cocoa/Cocoa.h>
#include "OgrePlatform.h"
#include "CollageApplication.h"
#include "Input.h"
#include "RenderEngine.h"
#include "UserInterface.h"
#include "System.h"
#include "MenuState.h"
#include "GameState.h"
#include "PauseState.h"
#include "Wallpaper.h"
#include "FrameCapture.h"
#include <QtGui>

CollageApplication::CollageApplication() {
}

CollageApplication::~CollageApplication() {
}

void CollageApplication::start() {
	//Wallpaper myWallPaperGenerator = Wallpaper();

	RenderEngine::Instance().initOgre("Collage");
	System::Instance().loadRecources();
	Input::Instance().initOis();
	UserInterface::Instance().initOgreBites();

	System::Instance().logMessage("Collage initialized");

	m_pAppStateManager = new AppStateManager();

	MenuState::create(m_pAppStateManager, "MenuState");
	GameState::create(m_pAppStateManager, "GameState");
	PauseState::create(m_pAppStateManager, "PauseState");

	m_pAppStateManager->start(m_pAppStateManager->findByName("MenuState"));

}

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE

static id mAppDelegate;

// All this does is suppress some messages in the run log.  NSApplication does not
// implement buttonPressed and apps without a NIB have no target for the action.
@implementation NSApplication (_suppressUnimplementedActionWarning)
- (void) buttonPressed:(id)sender
{
    /* Do nothing */
}
@end

@interface AppDelegate : NSObject <NSApplicationDelegate>
{
    NSTimer *mTimer;
    //OgreBites::SampleBrowser sb;
    CollageApplication collage;

    NSDate *mDate;
    NSTimeInterval mLastFrameTime;
}

- (void)go;
- (void)renderOneFrame:(id)sender;

@property (retain) NSTimer *mTimer;
@property (nonatomic) NSTimeInterval mLastFrameTime;

@end

@implementation AppDelegate

@synthesize mTimer;
@dynamic mLastFrameTime;

- (NSTimeInterval)mLastFrameTime
{
    return mLastFrameTime;
}

- (void)setLastFrameTime:(NSTimeInterval)frameInterval
{
    // Frame interval defines how many display frames must pass between each time the
    // display link fires. The display link will only fire 30 times a second when the
    // frame internal is two on a display that refreshes 60 times a second. The default
    // frame interval setting of one will fire 60 times a second when the display refreshes
    // at 60 times a second. A frame interval setting of less than one results in undefined
    // behavior.
    if (frameInterval >= 1)
    {
        mLastFrameTime = frameInterval;
    }
}

- (void)go {
    NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
    mLastFrameTime = 1;
    mTimer = nil;

    try {
        collage.start();
        
        Ogre::Root::getSingleton().getRenderSystem()->_initRenderTargets();
        
        // Clear event times
		Ogre::Root::getSingleton().clearEventTimes();
    } catch( Ogre::Exception& e ) {
        std::cerr << "An exception has occurred: " <<
        e.getFullDescription().c_str() << std::endl;
    }
    
    mTimer = [NSTimer scheduledTimerWithTimeInterval:(NSTimeInterval)(1.0 / 60.0) * mLastFrameTime
                                              target:self
                                            selector:@selector(renderOneFrame:)
                                            userInfo:nil
                                             repeats:YES];
    [pool release];
}
    
- (void)applicationDidFinishLaunching:(NSNotification *)application {
    mLastFrameTime = 1;
    mTimer = nil;

    [self go];
}
        
- (void)renderOneFrame:(id)sender
{
    if(Ogre::Root::getSingletonPtr() && Ogre::Root::getSingleton().isInitialised())
	{
		Input::Instance().m_pKeyboard->capture();
		Input::Instance().m_pMouse->capture();

		collage.m_pAppStateManager->m_ActiveStateStack.back()->update((Ogre::Real)[mTimer timeInterval]);

		RenderEngine::Instance().updateOgre((Ogre::Real)[mTimer timeInterval]);

		Ogre::Root::getSingleton().renderOneFrame((Ogre::Real)[mTimer timeInterval]);
    }
    else
    {
        [mTimer invalidate];
        mTimer = nil;
        [NSApp performSelector:@selector(terminate:) withObject:nil afterDelay:0.0];
    }
}

- (void)dealloc {
    if(mTimer)
    {
        [mTimer invalidate];
        mTimer = nil;
    }

    [super dealloc];
}

@end
#endif

int main(int argc, char *argv[]) {
	//Qt part

//	//QUrl url = QUrl("http://localhost/collage/");
//	//QUrl url = QUrl("http://en.wikipedia.org/wiki/Special:Random");
//	QUrl url = QUrl("http://wallbase.net/random");
//	//QUrl url = QUrl("http://wallbase2.net/high-resolution/1c648993030030fc43c6879b051bd5d9/wallpaper-247292.jpg");
//	QString fileName = "../Media/Textures/wall.png";
//
//	//"http://wallbase.net/wallpaper/"
//	QApplication a(argc, argv);
//	FrameCapture capture;
//	QObject::connect(&capture, SIGNAL(finished()), QApplication::instance(), SLOT(quit()));
//	capture.load(url, fileName);
//	//qDebug("image: %s", capture.getFirstAttribute("a","href","http://wallbase.net/wallpaper/"));
//	a.exec();

#if (OGRE_PLATFORM == OGRE_PLATFORM_APPLE) && __LP64__
	NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];

	mAppDelegate = [[AppDelegate alloc] init];
	[[NSApplication sharedApplication] setDelegate:mAppDelegate];
	int retVal = NSApplicationMain(argc, (const char **) argv);

	[pool release];

	return retVal;
#else
	CollageApplication collage;
	try {
		collage.start();
	}
	catch(const std::exception& e) {
		fprintf(stderr, "An exception has occurred: %s\n", e.what());
	}
#endif
}
