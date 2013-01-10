#ifndef EGE_CORE_SCENEMANAGER_H
#define EGE_CORE_SCENEMANAGER_H

#include <EGE.h>
#include <EGETime.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class IRenderer;
class SceneNode;
EGE_DECLARE_SMART_CLASS(SceneManager, PSceneManager)
EGE_DECLARE_SMART_CLASS(Camera, PCamera)
EGE_DECLARE_SMART_CLASS(Viewport, PViewport)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class SceneManager : public Object//ISceneNodeListener
{
  public:

    SceneManager(Application* app);
    virtual ~SceneManager();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /*! Creates object. */
    EGEResult construct();
    /*! Updates manager. */
    void update(const Time& time);
    /*! Destroys manager data effectively resetting it. */
    void destroy();

//    typedef vector<SceneNode*> SceneNodesVector;
 //   typedef vector<CLight*> LightsVector;

    /*! Returns root node. */
    SceneNode* rootNode() const { return m_rootNode; }

    //u32               getVisibleNodeCount( void ) const;                                      // gets number of visible objects
    //SceneNodesVector& getVisibleNodesVector( void );                                          // gets vector of visible nodes

    /*! Renders scene. 
     *  @param  camera    Camera from which point of view rendering is to be done.
     *  @param  viewport  Viewport for rendering.
     */
    void render(PCamera camera, PViewport viewport);

    //// render system related methods
    //void setRenderSystem( CRenderSystem* pcRenderSystem );                                    // sets render system

    //// render queue related methods
    //inline CRenderQueue* getRenderQueue( void ) const { return m_pcRenderQueue; }             // gets render queue

    //// sky related methods
    //bool setSkyBox( PMaterialResource& pcSkyBoxMaterial );                                 // creates sky box with given material
    //bool isSkyBoxEnabled( void ) const;                                                       // returns TRUE if sky box is enabled
    //void enableSkyBox( bool bEnable );                                                        // enables/disables sky box

    //// bounding box related methods
    //inline bool getShowBoundingBoxes( void ) const { return m_bShowBoundingBoxes; }           // returns TRUE if bounding boxes are visible
    //void        setShowBoundingBoxes( bool bEnable );                                         // enables/disables bounding boxes

    //// bounding sphere related methods
    //inline bool getShowBoundingSpheres( void ) const { return m_bShowBoundingSpheres; }       // returns TRUE if bounding spheres are visible
    //void        setShowBoundingSpheres( bool bEnable );                                       // enables/disables bounding spheres

    //// cursor related methods
    //bool setCursor( PCursorResource& cCursorResource );                                            
    //bool isCursorEnabled( void );
    //void enableCursor( bool bEnable );

    //// light related methods
    //CLight*       createLight( const string& strName );                                       // creates light, returns NULL if failed
    //void          destroyLight( CLight* pcLight );                                            // destroys given light
    //void          destroyAllLights( void );                                                   // destroys all lights
    //LightsVector& getLightsAffectingFrustum( void );                                          // gets vecotr of lights affecting the frustum

    //// auto uniforms related methods
    //CAutoUniformDataSource* getAutoUniformsDataSource( void ) const;                          // gets auto uniforms data source, returns NULL if error

  protected:

    //string m_strTypeName;                               // type name
    //string m_strName;                                   // name
    //

    /*! Root node. */
    SceneNode* m_rootNode;

    //CRenderSystem* m_pcRenderSystem;                    // pointer to render system

    //CRenderQueue* m_pcRenderQueue;                      // render queue

    //bool m_bResetModelViewMatrix;                       // TRUE if model view matrix has been overridden by renderable and should be reset to camera's one
    //bool m_bResetProjectionMatrix;                      // TRUE if projection matrix has been overridden by renderable and should be reset to camera's one
    //bool m_bShowBoundingBoxes;                          // TRUE if bounding boxes should be shown
    //bool m_bShowBoundingSpheres;                        // TRUE if bounding spheres should be shown

    //Camera* m_pcCamera;                                // camera in use

    //SceneNode* m_pcSkyBoxNode;                         // sky box node

    //SceneNodesVector m_vpcVisibleNodes;                 // vector of pointers to visible scene nodes

    //CCursor* m_pcCursor;                                // mouse cursor

    //CAutoUniformDataSource* m_pcAutoUniformDataSource;  // data source of auto uniforms to feed GPU parameters if needed

    //LightsVector m_vpcLights;                           // lights pool
    //LightsVector m_vpcLightsAffectingFrustum;           // vector of pointers to lights which affects frustum


    //// initializers
    //virtual bool initialize( void );                                // initializes object, returns TRUE if successful

    //// ISceneNodeListener overrides
    //virtual void onSceneNodeVisible( SceneNode* pcNode );          // notifies that given node is visible

    /*! Adds all visible object for rendering. 
     *  @param  camera    Camera from which point of view rendering is being done.
     *  @param  renderer  Renderer accepting the object to show.
     */
    void addForRendering(PCamera& camera, IRenderer* renderer);

    //// visibility related methods
    //void findLightsAffectingFrustum( void );                        // finds all lights affecting frustum

    //// render related methods
    //void renderVisibleObjects( void );                              // renders visible objects
    //void queueSkyForRendering( void );                              // adds sky for rendering if any
    //void queueLensFlaresForRendering( void );                       // adds lens flares for rendering if any
    //void queueMouseCursorForRendering( void );                      // mouse cursor for rendering if any

    //// matrix related methods
    //void useRenderableMatrices( const CRenderable* pcRenderable );  // applies transformation matrices of given renderable
    //void resetMatrices( void );                                     // resets transformation matrices to default ones (of camera)
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_SCENEMANAGER_H