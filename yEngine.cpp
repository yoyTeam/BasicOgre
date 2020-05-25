#include "yEngine.h"

#include "OgreGL3PlusPlugin.h"
#include "OgreGL3PlusRenderSystem.h"
#include "OgreParticleFXPlugin.h"

#include "OgreD3D11Plugin.h"
#include "OgreD3D11RenderSystem.h"
#include "OgreWindow.h"

#include "OgreTextureGpu.h"
#include "OgreTextureGpuManager.h"
#include "OgreProfiler.h"


yEngine::yEngine(){
}

yEngine::~yEngine(){
}

void yEngine::init(){

    //Init Ogre::Root, with no cfg Files
    mRoot = new Ogre::Root("","");

    //Install RenderSystem Plugin
//    Ogre::GL3PlusPlugin* glPlugin = OGRE_NEW Ogre::GL3PlusPlugin();
//    Ogre::Root::getSingleton().installPlugin(glPlugin);

    //Install RenderSystem Plugin
    Ogre::D3D11Plugin* d3dPlugin = OGRE_NEW Ogre::D3D11Plugin();
    mRoot->installPlugin(d3dPlugin);

    //Install ParticleFX Plugin
    Ogre::ParticleFXPlugin* pfxPlugin = OGRE_NEW Ogre::ParticleFXPlugin();
    mRoot->installPlugin(pfxPlugin);

    //Use the first available Renderer
    Ogre::RenderSystem* renderSys = mRoot->getAvailableRenderers()[0];
    mRoot->setRenderSystem(renderSys);

    //Initialize Root
    mRoot->initialise(false);
    mRoot->addFrameListener(this);

#ifdef USE_QT_WINDOW
    //Create a QtWindow with a widgetContainer
    mOgreWidget = new OgreWidget();
#else
    //Initialize renderWindow
    Ogre::NameValuePairList params;
    params["gamma"] = "true";   
    Ogre::Window* renderWindow = mRoot->createRenderWindow("OgreTest", 1280, 720, false, &params);
#endif

    //Create the SceneManager
    sceneManager = mRoot->createSceneManager( Ogre::ST_GENERIC, 4, "SceneManager" );

    //Create the MainCamera
    Ogre::Camera* mainCamera = sceneManager->createCamera("Camera");
    mainCamera->setAutoAspectRatio(true);
    mainCamera->setNearClipDistance(0.01);
    mainCamera->setFarClipDistance(1000.0);
    //And attach it to a SceneNode
    mainCameraNode = sceneManager->getRootSceneNode()->createChildSceneNode();
    mainCamera->detachFromParent();
    mainCameraNode->attachObject(mainCamera);
    mainCamera->setPosition(0,300,300);
    mainCamera->setDirection(Ogre::Vector3(0,0,0)-mainCamera->getPosition());


    //Create a directional Light
    Ogre::Light* directionalLight = sceneManager->createLight();
    directionalLight->setDiffuseColour(1, 1, 1);
    directionalLight->setType(Ogre::Light::LT_DIRECTIONAL);
    Ogre::SceneNode* lightNode = sceneManager->getRootSceneNode()->createChildSceneNode();
    lightNode->attachObject(directionalLight);
    directionalLight->setDirection(Ogre::Vector3(-1, -1, -1));
    directionalLight->setPowerScale(1);


    //Add media folder
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation("./media/","FileSystem");
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups( true );

    //Create a basic Workspace
    mRoot->getCompositorManager2()->createBasicWorkspaceDef("sceneworkspace", Ogre::ColourValue::Black);

#ifdef USE_QT_WINDOW
    mRoot->getCompositorManager2()->addWorkspace(sceneManager, mOgreWidget->mOgreRenderWindow->getTexture(), mainCamera, "sceneworkspace", true);
#else
    mRoot->getCompositorManager2()->addWorkspace(sceneManager, renderWindow->getTexture(), mainCamera, "sceneworkspace", true);
#endif

    //register HLMS
    Ogre::String rootHlmsFolder = "../../yoyEngine/media/";

    //For retrieval of the paths to the different folders needed
    Ogre::String mainFolderPath;
    Ogre::StringVector libraryFoldersPaths;
    Ogre::StringVector::const_iterator libraryFolderPathIt;
    Ogre::StringVector::const_iterator libraryFolderPathEn;

    Ogre::ArchiveManager &archiveManager = Ogre::ArchiveManager::getSingleton();

    {
        //Create & Register HlmsUnlit
        //Get the path to all the subdirectories used by HlmsUnlit
        Ogre::HlmsUnlit::getDefaultPaths( mainFolderPath, libraryFoldersPaths );
        Ogre::Archive *archiveUnlit = archiveManager.load( rootHlmsFolder + mainFolderPath,
                                                           "FileSystem", true );
        Ogre::ArchiveVec archiveUnlitLibraryFolders;
        libraryFolderPathIt = libraryFoldersPaths.begin();
        libraryFolderPathEn = libraryFoldersPaths.end();
        while( libraryFolderPathIt != libraryFolderPathEn )
        {
            Ogre::Archive *archiveLibrary =
                    archiveManager.load( rootHlmsFolder + *libraryFolderPathIt, "FileSystem", true );
            archiveUnlitLibraryFolders.push_back( archiveLibrary );

            ++libraryFolderPathIt;
        }

        //Create and register the unlit Hlms
        mHlmsUnlit = OGRE_NEW Ogre::HlmsUnlit( archiveUnlit, &archiveUnlitLibraryFolders );
        Ogre::Root::getSingleton().getHlmsManager()->registerHlms( mHlmsUnlit );
    }

    {
        //Create & Register HlmsPbs
        //Do the same for HlmsPbs:
        Ogre::HlmsPbs::getDefaultPaths( mainFolderPath, libraryFoldersPaths );
        Ogre::Archive *archivePbs = archiveManager.load( rootHlmsFolder + mainFolderPath,
                                                         "FileSystem", true );

        //Get the library archive(s)
        Ogre::ArchiveVec archivePbsLibraryFolders;
        libraryFolderPathIt = libraryFoldersPaths.begin();
        libraryFolderPathEn = libraryFoldersPaths.end();
        while( libraryFolderPathIt != libraryFolderPathEn )
        {
            Ogre::Archive *archiveLibrary =
                    archiveManager.load( rootHlmsFolder + *libraryFolderPathIt, "FileSystem", true );
            archivePbsLibraryFolders.push_back( archiveLibrary );
            ++libraryFolderPathIt;
        }

        //Create and register
        mHlmsPbs = OGRE_NEW Ogre::HlmsPbs( archivePbs, &archivePbsLibraryFolders );
        Ogre::Root::getSingleton().getHlmsManager()->registerHlms( mHlmsPbs );
    }



    //configure shadow quality
    mHlmsPbs->setShadowSettings(Ogre::HlmsPbs::PCF_4x4);

    //Get the cube Mesh
    Ogre::v1::MeshPtr cubeMesh = Ogre::v1::MeshManager::getSingletonPtr()->load("cube.mesh",Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    cubeMeshV2 = Ogre::MeshManager::getSingleton().createManual( "cubeV2.mesh", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME );
    //Import the v1 mesh to v2 format
    cubeMeshV2->importV1( cubeMesh.get(), true, true, true );
    cubeMesh->unload();

    //create a datablock (material)
    Ogre::String datablockName = "CubeMat";
    Ogre::HlmsPbsDatablock* cubeDatablock = static_cast<Ogre::HlmsPbsDatablock*>(
                                          mHlmsPbs->createDatablock( datablockName,
                                          datablockName,
                                          Ogre::HlmsMacroblock(),
                                          Ogre::HlmsBlendblock(),
                                          Ogre::HlmsParamVec() ) );

    //set the diffuse texture
    Ogre::TextureGpuManager *textureManager = Ogre::Root::getSingletonPtr()->getRenderSystem()->getTextureGpuManager();
    Ogre::TextureGpu* diffuseTexture = textureManager->createOrRetrieveTexture(
                                        "grid.png",
                                        Ogre::GpuPageOutStrategy::Discard,
                                        Ogre::TextureFlags::AutomaticBatching | Ogre::TextureFlags::PrefersLoadingFromFileAsSRGB,
                                        Ogre::TextureTypes::Type2D,
                                        Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME );
    diffuseTexture->scheduleTransitionTo( Ogre::GpuResidency::Resident );
    cubeDatablock->setTexture( Ogre::PBSM_DIFFUSE, diffuseTexture );

    //set some values
    cubeDatablock->setDiffuse(Ogre::Vector3(1,1,1));
    cubeDatablock->setFresnel(Ogre::Vector3::UNIT_SCALE*0.1,false);
    cubeDatablock->setSpecular(Ogre::Vector3(0,0,0));
    cubeDatablock->setRoughness(0.9);

    //Create a Cube
    Ogre::Item* cubeItem = sceneManager->createItem(cubeMeshV2);
    cubeNode = sceneManager->getRootSceneNode()->createChildSceneNode();
    cubeNode->attachObject(cubeItem);
    cubeNode->setScale(Ogre::Vector3(0.6,0.25,1.5));
    //cubeNode->scale( Ogre::Vector3::UNIT_SCALE*0.2 );
    cubeNode->setPosition(0,0,0);
    cubeItem->setDatablock(cubeDatablock);

    //some child cubes to make it look like a plane
    {
        Ogre::Item* cubeItem = sceneManager->createItem(cubeMeshV2);
        Ogre::SceneNode* childCubeNode = cubeNode->createChildSceneNode();
        childCubeNode->attachObject(cubeItem);
        childCubeNode->scale(Ogre::Vector3(2,0.1,0.4));
        cubeItem->setDatablock(cubeDatablock);
    }
    {
        Ogre::Item* cubeItem = sceneManager->createItem(cubeMeshV2);
        Ogre::SceneNode* childCubeNode = cubeNode->createChildSceneNode();
        childCubeNode->attachObject(cubeItem);
        childCubeNode->scale(Ogre::Vector3(0.1,2,1));
        cubeItem->setDatablock(cubeDatablock);
    }



    //emit some particles
    Ogre::ParticleSystem* newParticleSystem = sceneManager->createParticleSystem( 1000 );
    newParticleSystem->setDefaultDimensions(50,50);
    newParticleSystem->setIterationInterval(0.033);
    newParticleSystem->setKeepParticlesInLocalSpace(false);

    Ogre::SceneNode* node = cubeNode->createChildSceneNode();
    node->scale(Ogre::Vector3::UNIT_SCALE);
    node->setPosition(0,0,0);
    node->attachObject( newParticleSystem );

    Ogre::BoxEmitter* newEmitter = (Ogre::BoxEmitter*)newParticleSystem->addEmitter("Box");
    newEmitter->setWidth( 1 );
    newEmitter->setHeight( 1 );
    newEmitter->setDepth( 1 );
    newEmitter->setTimeToLive( 3 );
    newEmitter->setParticleVelocity( 200 );
    newEmitter->setEmissionRate( 10 );

    Ogre::HlmsUnlitDatablock *datablock = static_cast<Ogre::HlmsUnlitDatablock*>( Ogre::Root::getSingletonPtr()->getHlmsManager()->getHlms( Ogre::HLMS_UNLIT )->createDatablock( "particleMat", "particleMat", Ogre::HlmsMacroblock(), Ogre::HlmsBlendblock(), Ogre::HlmsParamVec()) );

    newParticleSystem->setMaterialName("particleMat");

    Ogre::Profiler::getSingleton().setEnabled(true);

}

void yEngine::startRender(){
    mRoot->startRendering();
}

bool yEngine::frameRenderingQueued(const Ogre::FrameEvent &evt){
    //rotate the cube
    cubeNode->rotate( Ogre::Quaternion( Ogre::Radian( Ogre::Degree( 50 * evt.timeSinceLastFrame ).valueRadians() ), Ogre::Vector3::UNIT_Y ) );

    return true;
}
