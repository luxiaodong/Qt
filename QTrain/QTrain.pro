#-------------------------------------------------
#
# Project created by QtCreator 2011-02-07T22:39:25
#
#-------------------------------------------------

QT       += core gui
QT += sql

TARGET = QTrain
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    cocos2dx/zstream_test.cpp \
    cocos2dx/ZipUtils.cpp \
    cocos2dx/zfstream.cpp \
    cocos2dx/unzip.cpp \
    cocos2dx/TransformUtils.cpp \
    cocos2dx/TGAlib.cpp \
    cocos2dx/test.cpp \
    cocos2dx/platform.cpp \
    cocos2dx/NewDeleteOp.cpp \
    cocos2dx/ioapi.cpp \
    cocos2dx/Cocos2dJni.cpp \
    cocos2dx/cocos2d.cpp \
    cocos2dx/CCZone.cpp \
    cocos2dx/ccUtils.cpp \
    cocos2dx/CCUserDefault.cpp \
    cocos2dx/CCTransitionRadial.cpp \
    cocos2dx/CCTransitionPageTurn.cpp \
    cocos2dx/CCTransition.cpp \
    cocos2dx/CCTouchHandler.cpp \
    cocos2dx/CCTouchDispatcher.cpp \
    cocos2dx/CCTMXXMLParser.cpp \
    cocos2dx/CCTMXTiledMap.cpp \
    cocos2dx/CCTMXObjectGroup.cpp \
    cocos2dx/CCTMXLayer.cpp \
    cocos2dx/CCTileMapAtlas.cpp \
    cocos2dx/CCThread_wophone.cpp \
    cocos2dx/CCThread_win32.cpp \
    cocos2dx/CCThread_android.cpp \
    cocos2dx/CCThread.cpp \
    cocos2dx/CCTextureCache.cpp \
    cocos2dx/CCTextureAtlas.cpp \
    cocos2dx/CCTexture2D.cpp \
    cocos2dx/CCTextFieldTTF.cpp \
    cocos2dx/CCStdC.cpp \
    cocos2dx/CCSpriteSheet.cpp \
    cocos2dx/CCSpriteFrameCache.cpp \
    cocos2dx/CCSpriteFrame.cpp \
    cocos2dx/CCSpriteBatchNode.cpp \
    cocos2dx/CCSprite.cpp \
    cocos2dx/CCSet.cpp \
    cocos2dx/CCScheduler.cpp \
    cocos2dx/CCScene.cpp \
    cocos2dx/CCSAXParser_airplay.cpp \
    cocos2dx/CCSAXParser.cpp \
    cocos2dx/CCRibbon.cpp \
    cocos2dx/CCRenderTexture.cpp \
    cocos2dx/CCPVRTexture.cpp \
    cocos2dx/CCProgressTimer.cpp \
    cocos2dx/CCProfiling.cpp \
    cocos2dx/CCPointExtension.cpp \
    cocos2dx/CCParticleSystemQuad.cpp \
    cocos2dx/CCParticleSystemPoint.cpp \
    cocos2dx/CCParticleSystem.cpp \
    cocos2dx/CCParticleExamples.cpp \
    cocos2dx/CCParallaxNode.cpp \
    cocos2dx/CCObject.cpp \
    cocos2dx/CCNS.cpp \
    cocos2dx/CCNode.cpp \
    cocos2dx/CCMotionStreak.cpp \
    cocos2dx/CCMenuItem.cpp \
    cocos2dx/CCMenu.cpp \
    cocos2dx/CCLock.cpp \
    cocos2dx/CCLayer.cpp \
    cocos2dx/CCLabelTTF.cpp \
    cocos2dx/CCLabelBMFont.cpp \
    cocos2dx/CCLabelAtlas.cpp \
    cocos2dx/CCKeypadDispatcher.cpp \
    cocos2dx/CCKeypadDelegate.cpp \
    cocos2dx/CCIMEDispatcher.cpp \
    cocos2dx/CCImage_wophone.cpp \
    cocos2dx/CCImage_win32.cpp \
    cocos2dx/CCImage_android.cpp \
    cocos2dx/CCImage_airplay.cpp \
    cocos2dx/CCImage.cpp \
    cocos2dx/CCGrid.cpp \
    cocos2dx/CCGrabber.cpp \
    cocos2dx/CCGL.cpp \
    cocos2dx/CCGeometry.cpp \
    cocos2dx/CCFileUtils_wophone.cpp \
    cocos2dx/CCFileUtils_win32.cpp \
    cocos2dx/CCFileUtils_android.cpp \
    cocos2dx/CCFileUtils_airplay.cpp \
    cocos2dx/CCFileUtils.cpp \
    cocos2dx/CCEGLView_wophone.cpp \
    cocos2dx/CCEGLView_win32.cpp \
    cocos2dx/CCEGLView_android.cpp \
    cocos2dx/CCEGLView_airplay.cpp \
    cocos2dx/CCDrawingPrimitives.cpp \
    cocos2dx/CCDirector.cpp \
    cocos2dx/CCData.cpp \
    cocos2dx/CCConfiguration.cpp \
    cocos2dx/CCCommon.cpp \
    cocos2dx/CCCamera.cpp \
    cocos2dx/CCAutoreleasePool.cpp \
    cocos2dx/CCAtlasNode.cpp \
    cocos2dx/CCArray.cpp \
    cocos2dx/CCApplication_wophone.cpp \
    cocos2dx/CCApplication_win32.cpp \
    cocos2dx/CCApplication_android.cpp \
    cocos2dx/CCApplication_airplay.cpp \
    cocos2dx/CCAnimationCache.cpp \
    cocos2dx/CCAnimation.cpp \
    cocos2dx/CCAffineTransform.cpp \
    cocos2dx/CCActionTiledGrid.cpp \
    cocos2dx/CCActionProgressTimer.cpp \
    cocos2dx/CCActionPageTurn3D.cpp \
    cocos2dx/CCActionManager.cpp \
    cocos2dx/CCActionInterval.cpp \
    cocos2dx/CCActionInstant.cpp \
    cocos2dx/CCActionGrid.cpp \
    cocos2dx/CCActionGrid3D.cpp \
    cocos2dx/CCActionEase.cpp \
    cocos2dx/CCActionCamera.cpp \
    cocos2dx/CCAction.cpp \
    cocos2dx/CCAccelerometer_wophone.cpp \
    cocos2dx/CCAccelerometer_android.cpp \
    cocos2dx/CCAccelerometer_airplay.cpp \
    cocos2dx/base64.cpp \
    Classes/HelloWorldScene.cpp \
    Classes/HelloWorldLayer.cpp \
    Classes/GTutorialScene.cpp \
    Classes/GTutorialLayer.cpp \
    Classes/GStateMachine.cpp \
    Classes/GSettingScene.cpp \
    Classes/GSettingLayer.cpp \
    Classes/GPlayTargetLayer.cpp \
    Classes/GPlayScene.cpp \
    Classes/GPlayResource.cpp \
    Classes/GPlayLayer.cpp \
    Classes/GPlayItemLayer.cpp \
    Classes/GPlayGardenData.cpp \
    Classes/GPlayFlowerPot.cpp \
    Classes/GPlayFlowerData.cpp \
    Classes/GPlayFlower.cpp \
    Classes/GPlayFingerTrace.cpp \
    Classes/GPauseLayer.cpp \
    Classes/GNumberLayer.cpp \
    Classes/GMainMenuScene.cpp \
    Classes/GMainMenuLayer.cpp \
    Classes/GLevelSelectScene.cpp \
    Classes/GLevelSelectLayer.cpp \
    Classes/GLevelInfoLayer.cpp \
    Classes/GLevelConfig.cpp \
    Classes/GGardenLayer.cpp \
    Classes/GGameOverLayer.cpp \
    Classes/GFlowerPickUnit.cpp \
    Classes/GDataManage.cpp \
    Classes/GCompletedLayer.cpp \
    Classes/GChallengeLayer.cpp \
    Classes/GardenAppDelegate.cpp \
    Classes/GLayout.cpp \
    Classes/GSound.cpp \
    GParticles.cpp \
    GPanelLayer.cpp \
    GPanelData.cpp \
    GParticlesData.cpp \
    GBeatSequence.cpp \
    Classes/GPlayerRecordLayer.cpp \
    Classes/GLevelDayLayer.cpp \
    Classes/GKindConfig.cpp \
    Classes/GCustomProjection.cpp \
    Classes/GChallengeConfig.cpp \
    Classes/GCentipeSegment.cpp \
    Classes/GCentipe.cpp \
    Classes/GBossCentipe.cpp

HEADERS  += mainwindow.h \
    cocos2dx/zutil.h \
    cocos2dx/zstream.h \
    cocos2dx/zlib.h \
    cocos2dx/ZipUtils.h \
    cocos2dx/zip.h \
    cocos2dx/zfstream.h \
    cocos2dx/zconf.in.h \
    cocos2dx/zconf.h \
    cocos2dx/xpointer.h \
    cocos2dx/xpathInternals.h \
    cocos2dx/xpath.h \
    cocos2dx/xmlwriter.h \
    cocos2dx/xmlversion.h \
    cocos2dx/xmlurl.h \
    cocos2dx/xmlunicode.h \
    cocos2dx/xmltok_impl.h \
    cocos2dx/xmltok.h \
    cocos2dx/xmltchar.h \
    cocos2dx/xmlstring.h \
    cocos2dx/xmlschemastypes.h \
    cocos2dx/xmlschemas.h \
    cocos2dx/xmlsave.h \
    cocos2dx/xmlrole.h \
    cocos2dx/xmlregexp.h \
    cocos2dx/xmlreader.h \
    cocos2dx/xmlmodule.h \
    cocos2dx/xmlmime.h \
    cocos2dx/xmlmemory.h \
    cocos2dx/xmlIO.h \
    cocos2dx/xmlfile.h \
    cocos2dx/xmlexports.h \
    cocos2dx/xmlerror.h \
    cocos2dx/xmlautomata.h \
    cocos2dx/xlink.h \
    cocos2dx/xinclude.h \
    cocos2dx/winconfig.h \
    cocos2dx/valid.h \
    cocos2dx/utlist.h \
    cocos2dx/uthash.h \
    cocos2dx/utf8tab.h \
    cocos2dx/uri.h \
    cocos2dx/unzip.h \
    cocos2dx/types.h \
    cocos2dx/typecheck-gcc.h \
    cocos2dx/trees.h \
    cocos2dx/tree.h \
    cocos2dx/TransformUtils.h \
    cocos2dx/threads.h \
    cocos2dx/TGAlib.h \
    cocos2dx/stdcheaders.h \
    cocos2dx/SkXMLWriter.h \
    cocos2dx/SkXMLParser.h \
    cocos2dx/SkXfermode.h \
    cocos2dx/SkWriter32.h \
    cocos2dx/SkWindow.h \
    cocos2dx/SkWidgetViews.h \
    cocos2dx/SkWidget.h \
    cocos2dx/SkViewInflate.h \
    cocos2dx/SkView.h \
    cocos2dx/SkUtils.h \
    cocos2dx/SkUserConfig.h \
    cocos2dx/SkUnPreMultiply.h \
    cocos2dx/SkUnitMappers.h \
    cocos2dx/SkUnitMapper.h \
    cocos2dx/SkTypes.h \
    cocos2dx/SkTypeface.h \
    cocos2dx/SkTSearch.h \
    cocos2dx/SkTRegistry.h \
    cocos2dx/SkTransparentShader.h \
    cocos2dx/SkTime.h \
    cocos2dx/SkThread_platform.h \
    cocos2dx/SkThread.h \
    cocos2dx/SkTextLayout.h \
    cocos2dx/SkTextFormatParams.h \
    cocos2dx/SkTextBox.h \
    cocos2dx/SkTemplates.h \
    cocos2dx/SkTDStack.h \
    cocos2dx/SkTDict.h \
    cocos2dx/SkTDArray.h \
    cocos2dx/SkTableMaskFilter.h \
    cocos2dx/SkSystemEventTypes.h \
    cocos2dx/SkSVGTypes.h \
    cocos2dx/SkSVGParser.h \
    cocos2dx/SkSVGPaintState.h \
    cocos2dx/SkSVGBase.h \
    cocos2dx/SkSVGAttribute.h \
    cocos2dx/SkStroke.h \
    cocos2dx/SkString.h \
    cocos2dx/SkStream_Win.h \
    cocos2dx/SkStream.h \
    cocos2dx/SkStackViewLayout.h \
    cocos2dx/SkSize.h \
    cocos2dx/SkShape.h \
    cocos2dx/SkShader.h \
    cocos2dx/SkSfntUtils.h \
    cocos2dx/SkScrollBarView.h \
    cocos2dx/SkScan.h \
    cocos2dx/SkScalerContext.h \
    cocos2dx/SkScalarCompare.h \
    cocos2dx/SkScalar.h \
    cocos2dx/SkRegion.h \
    cocos2dx/SkRefCnt.h \
    cocos2dx/SkRectShape.h \
    cocos2dx/SkRect.h \
    cocos2dx/SkReader32.h \
    cocos2dx/SkRasterizer.h \
    cocos2dx/SkRandom.h \
    cocos2dx/SkPtrRecorder.h \
    cocos2dx/SkProxyCanvas.h \
    cocos2dx/SkProgressBarView.h \
    cocos2dx/SkPreConfig.h \
    cocos2dx/SkPostConfig.h \
    cocos2dx/SkPorterDuff.h \
    cocos2dx/SkPoint.h \
    cocos2dx/SkPixelXorXfermode.h \
    cocos2dx/SkPixelRef.h \
    cocos2dx/SkPicture.h \
    cocos2dx/SkPerspIter.h \
    cocos2dx/SkPDFTypes.h \
    cocos2dx/SkPDFStream.h \
    cocos2dx/SkPDFPage.h \
    cocos2dx/SkPDFImage.h \
    cocos2dx/SkPDFGraphicState.h \
    cocos2dx/SkPDFFormXObject.h \
    cocos2dx/SkPDFFont.h \
    cocos2dx/SkPDFDocument.h \
    cocos2dx/SkPDFDevice.h \
    cocos2dx/SkPDFCatalog.h \
    cocos2dx/SkPathMeasure.h \
    cocos2dx/SkPathEffect.h \
    cocos2dx/SkPath.h \
    cocos2dx/SkParsePath.h \
    cocos2dx/SkParsePaint.h \
    cocos2dx/SkParse.h \
    cocos2dx/SkPaintFlagsDrawFilter.h \
    cocos2dx/SkPaint.h \
    cocos2dx/SkPageFlipper.h \
    cocos2dx/SkPackBits.h \
    cocos2dx/SkOSWindow_wxwidgets.h \
    cocos2dx/SkOSWindow_Win.h \
    cocos2dx/SkOSWindow_Unix.h \
    cocos2dx/SkOSWindow_SDL.h \
    cocos2dx/SkOSWindow_Mac.h \
    cocos2dx/SkOSSound.h \
    cocos2dx/SkOSMenu.h \
    cocos2dx/SkOSFile.h \
    cocos2dx/SkNWayCanvas.h \
    cocos2dx/SkNinePatch.h \
    cocos2dx/SkMovie.h \
    cocos2dx/SkMMapStream.h \
    cocos2dx/SkMetaData.h \
    cocos2dx/SkMeshUtils.h \
    cocos2dx/SkMatrix.h \
    cocos2dx/SkMath.h \
    cocos2dx/SkMaskFilter.h \
    cocos2dx/SkMask.h \
    cocos2dx/SkMallocPixelRef.h \
    cocos2dx/SkLineClipper.h \
    cocos2dx/SkLayerRasterizer.h \
    cocos2dx/SkLayerDrawLooper.h \
    cocos2dx/SkLayer.h \
    cocos2dx/SkKey.h \
    cocos2dx/SkKernel33MaskFilter.h \
    cocos2dx/SkJS.h \
    cocos2dx/SkJpegUtility.h \
    cocos2dx/SkInterpolator.h \
    cocos2dx/SkImageView.h \
    cocos2dx/SkImageRef_GlobalPool.h \
    cocos2dx/SkImageRef.h \
    cocos2dx/SkImageEncoder.h \
    cocos2dx/SkImageDecoder.h \
    cocos2dx/SkHarfBuzzFont.h \
    cocos2dx/SkGroupShape.h \
    cocos2dx/SkGraphics.h \
    cocos2dx/SkGradientShader.h \
    cocos2dx/SkGlobals.h \
    cocos2dx/SkGLCanvas.h \
    cocos2dx/SkGeometry.h \
    cocos2dx/SkFontHost.h \
    cocos2dx/SkFloatingPoint.h \
    cocos2dx/SkFloatBits.h \
    cocos2dx/SkFlipPixelRef.h \
    cocos2dx/SkFlattenable.h \
    cocos2dx/SkFlate.h \
    cocos2dx/SkFixed.h \
    cocos2dx/SkFDot6.h \
    cocos2dx/SkEventSink.h \
    cocos2dx/SkEvent.h \
    cocos2dx/SkEndian.h \
    cocos2dx/SkEmbossMaskFilter.h \
    cocos2dx/SkEdgeClipper.h \
    cocos2dx/SkDumpCanvas.h \
    cocos2dx/SkDrawLooper.h \
    cocos2dx/SkDrawFilter.h \
    cocos2dx/SkDrawExtraPathEffect.h \
    cocos2dx/SkDraw.h \
    cocos2dx/SkDOM.h \
    cocos2dx/SkDither.h \
    cocos2dx/SkDiscretePathEffect.h \
    cocos2dx/SkDevice.h \
    cocos2dx/SkDescriptor.h \
    cocos2dx/SkDeque.h \
    cocos2dx/SkDashPathEffect.h \
    cocos2dx/SkCullPoints.h \
    cocos2dx/SkCubicInterval.h \
    cocos2dx/SkCornerPathEffect.h \
    cocos2dx/SkComposeShader.h \
    cocos2dx/SkColorShader.h \
    cocos2dx/SkColorPriv.h \
    cocos2dx/SkColorMatrixFilter.h \
    cocos2dx/SkColorMatrix.h \
    cocos2dx/SkColorFilter.h \
    cocos2dx/SkColor.h \
    cocos2dx/SkChunkAlloc.h \
    cocos2dx/SkCGUtils.h \
    cocos2dx/SkCanvas.h \
    cocos2dx/SkCamera.h \
    cocos2dx/SkBuffer.h \
    cocos2dx/SkBounder.h \
    cocos2dx/SkBoundaryPatch.h \
    cocos2dx/SkBorderView.h \
    cocos2dx/SkBML_XMLParser.h \
    cocos2dx/SkBML_WXMLParser.h \
    cocos2dx/SkBlurMaskFilter.h \
    cocos2dx/SkBlurDrawLooper.h \
    cocos2dx/SkBlitter.h \
    cocos2dx/SkBlitRow.h \
    cocos2dx/SkBitmap.h \
    cocos2dx/SkBGViewArtist.h \
    cocos2dx/SkAvoidXfermode.h \
    cocos2dx/SkAutoKern.h \
    cocos2dx/SkApplication.h \
    cocos2dx/SkAnimatorView.h \
    cocos2dx/SkAnimator.h \
    cocos2dx/Sk64.h \
    cocos2dx/Sk2DPathEffect.h \
    cocos2dx/Sk1DPathEffect.h \
    cocos2dx/selector_protocol.h \
    cocos2dx/schematron.h \
    cocos2dx/schemasInternals.h \
    cocos2dx/SAX.h \
    cocos2dx/SAX2.h \
    cocos2dx/relaxng.h \
    cocos2dx/puff.h \
    cocos2dx/pngconf.h \
    cocos2dx/png.h \
    cocos2dx/platform.h \
    cocos2dx/pattern.h \
    cocos2dx/parserInternals.h \
    cocos2dx/parser.h \
    cocos2dx/OpenGL_Internal.h \
    cocos2dx/nanohttp.h \
    cocos2dx/nanoftp.h \
    cocos2dx/nametab.h \
    cocos2dx/mztools.h \
    cocos2dx/multi.h \
    cocos2dx/mprintf.h \
    cocos2dx/minicheck.h \
    cocos2dx/macconfig.h \
    cocos2dx/list.h \
    cocos2dx/latin1tab.h \
    cocos2dx/khrplatform.h \
    cocos2dx/jpeglib.h \
    cocos2dx/jmorecfg.h \
    cocos2dx/jerror.h \
    cocos2dx/jconfig_win.h \
    cocos2dx/jconfig_linux.h \
    cocos2dx/jconfig.h \
    cocos2dx/iowin32.h \
    cocos2dx/ioapi.h \
    cocos2dx/internal.h \
    cocos2dx/inftrees.h \
    cocos2dx/inftree9.h \
    cocos2dx/inflate.h \
    cocos2dx/inflate9.h \
    cocos2dx/inffixed.h \
    cocos2dx/inffix9.h \
    cocos2dx/inffast.h \
    cocos2dx/infback9.h \
    cocos2dx/iconv.h \
    cocos2dx/iasciitab.h \
    cocos2dx/HTMLtree.h \
    cocos2dx/HTMLparser.h \
    cocos2dx/hash.h \
    cocos2dx/gzlog.h \
    cocos2dx/glplatform.h \
    cocos2dx/globals.h \
    cocos2dx/glext.h \
    cocos2dx/gl.h \
    cocos2dx/filemap.h \
    cocos2dx/expat_external.h \
    cocos2dx/expat_config.h \
    cocos2dx/expat.h \
    cocos2dx/ESRenderer.h \
    cocos2dx/ES1Renderer.h \
    cocos2dx/entities.h \
    cocos2dx/encoding.h \
    cocos2dx/eglplatform.h \
    cocos2dx/eglext.h \
    cocos2dx/egl.h \
    cocos2dx/easy.h \
    cocos2dx/EAGLView.h \
    cocos2dx/DOCBparser.h \
    cocos2dx/dict.h \
    cocos2dx/deflate.h \
    cocos2dx/debugXML.h \
    cocos2dx/curlver.h \
    cocos2dx/curlrules.h \
    cocos2dx/curlbuild.h \
    cocos2dx/curl.h \
    cocos2dx/crypt.h \
    cocos2dx/crc32.h \
    cocos2dx/codepage.h \
    cocos2dx/Cocos2dJni.h \
    cocos2dx/cocos2d.h \
    cocos2dx/chvalid.h \
    cocos2dx/chardata.h \
    cocos2dx/CCZone.h \
    cocos2dx/ccUtils.h \
    cocos2dx/CCUserDefault.h \
    cocos2dx/ccTypes.h \
    cocos2dx/CCTransitionRadial.h \
    cocos2dx/CCTransitionPageTurn.h \
    cocos2dx/CCTransition.h \
    cocos2dx/CCTouchHandler.h \
    cocos2dx/CCTouchDispatcher.h \
    cocos2dx/CCTouchDelegateProtocol.h \
    cocos2dx/CCTouch.h \
    cocos2dx/CCTMXXMLParser.h \
    cocos2dx/CCTMXTiledMap.h \
    cocos2dx/CCTMXObjectGroup.h \
    cocos2dx/CCTMXLayer.h \
    cocos2dx/CCTileMapAtlas.h \
    cocos2dx/CCThread.h \
    cocos2dx/CCTextureCache.h \
    cocos2dx/CCTextureAtlas.h \
    cocos2dx/CCTexture2D.h \
    cocos2dx/CCTextFieldTTF.h \
    cocos2dx/CCString.h \
    cocos2dx/CCStdC.h \
    cocos2dx/CCSpriteSheet.h \
    cocos2dx/CCSpriteFrameCache.h \
    cocos2dx/CCSpriteFrame.h \
    cocos2dx/CCSpriteBatchNode.h \
    cocos2dx/CCSprite.h \
    cocos2dx/CCSet.h \
    cocos2dx/CCScheduler.h \
    cocos2dx/CCScene.h \
    cocos2dx/CCSAXParser.h \
    cocos2dx/CCRibbon.h \
    cocos2dx/CCRenderTexture.h \
    cocos2dx/CCPVRTexture.h \
    cocos2dx/CCProtocols.h \
    cocos2dx/CCProgressTimer.h \
    cocos2dx/CCProfiling.h \
    cocos2dx/CCPointExtension.h \
    cocos2dx/CCPlatformMacros.h \
    cocos2dx/CCPlatformConfig.h \
    cocos2dx/CCParticleSystemQuad.h \
    cocos2dx/CCParticleSystemPoint.h \
    cocos2dx/CCParticleSystem.h \
    cocos2dx/CCParticleExamples.h \
    cocos2dx/CCParallaxNode.h \
    cocos2dx/CCObject.h \
    cocos2dx/CCNS.h \
    cocos2dx/CCNode.h \
    cocos2dx/CCMutableDictionary.h \
    cocos2dx/CCMutableArray.h \
    cocos2dx/CCMotionStreak.h \
    cocos2dx/CCMenuItem.h \
    cocos2dx/CCMenu.h \
    cocos2dx/ccMacros.h \
    cocos2dx/CCLock.h \
    cocos2dx/CCLibxml2.h \
    cocos2dx/CCLayer.h \
    cocos2dx/CCLabelTTF.h \
    cocos2dx/CCLabelBMFont.h \
    cocos2dx/CCLabelAtlas.h \
    cocos2dx/CCKeypadDispatcher.h \
    cocos2dx/CCKeypadDelegate.h \
    cocos2dx/CCIMEDispatcher.h \
    cocos2dx/CCIMEDelegate.h \
    cocos2dx/CCImage.h \
    cocos2dx/CCGrid.h \
    cocos2dx/CCGrabber.h \
    cocos2dx/CCGL.h \
    cocos2dx/CCGeometry.h \
    cocos2dx/CCFileUtils.h \
    cocos2dx/CCEGLView_wophone.h \
    cocos2dx/CCEGLView_win32.h \
    cocos2dx/CCEGLView_platform.h \
    cocos2dx/CCEGLView_ios.h \
    cocos2dx/CCEGLView_android.h \
    cocos2dx/CCEGLView_airplay.h \
    cocos2dx/CCEGLView.h \
    cocos2dx/CCDrawingPrimitives.h \
    cocos2dx/CCDirectorCaller.h \
    cocos2dx/CCDirector.h \
    cocos2dx/CCData.h \
    cocos2dx/CCConfiguration.h \
    cocos2dx/ccConfig.h \
    cocos2dx/CCCommon.h \
    cocos2dx/ccCArray.h \
    cocos2dx/CCCamera.h \
    cocos2dx/CCAutoreleasePool.h \
    cocos2dx/CCAtlasNode.h \
    cocos2dx/CCArray.h \
    cocos2dx/CCArchOptimalParticleSystem.h \
    cocos2dx/CCApplication_wophone.h \
    cocos2dx/CCApplication_win32.h \
    cocos2dx/CCApplication_platform.h \
    cocos2dx/CCApplication_ios.h \
    cocos2dx/CCApplication_android.h \
    cocos2dx/CCApplication_airplay.h \
    cocos2dx/CCApplication.h \
    cocos2dx/CCAnimationCache.h \
    cocos2dx/CCAnimation.h \
    cocos2dx/CCAffineTransform.h \
    cocos2dx/CCActionTiledGrid.h \
    cocos2dx/CCActionProgressTimer.h \
    cocos2dx/CCActionPageTurn3D.h \
    cocos2dx/CCActionManager.h \
    cocos2dx/CCActionInterval.h \
    cocos2dx/CCActionInstant.h \
    cocos2dx/CCActionGrid.h \
    cocos2dx/CCActionGrid3D.h \
    cocos2dx/CCActionEase.h \
    cocos2dx/CCActionCamera.h \
    cocos2dx/CCAction.h \
    cocos2dx/CCAccelerometerDelegate.h \
    cocos2dx/CCAccelerometer_wophone.h \
    cocos2dx/CCAccelerometer_win32.h \
    cocos2dx/CCAccelerometer_platform.h \
    cocos2dx/CCAccelerometer_ios.h \
    cocos2dx/CCAccelerometer_android.h \
    cocos2dx/CCAccelerometer_airplay.h \
    cocos2dx/CCAccelerometer.h \
    cocos2dx/catalog.h \
    cocos2dx/c14n.h \
    cocos2dx/blast.h \
    cocos2dx/base64.h \
    cocos2dx/asciitab.h \
    cocos2dx/ascii.h \
    cocos2dx/AccelerometerDelegateWrapper.h \
    Classes/HelloWorldLayer.h \
    Classes/GTutorialScene.h \
    Classes/GTutorialLayer.h \
    Classes/GStateMachine.h \
    Classes/GSettingScene.h \
    Classes/GSettingLayer.h \
    Classes/GPlayTargetLayer.h \
    Classes/GPlayScene.h \
    Classes/GPlayResource.h \
    Classes/GPlayLayer.h \
    Classes/GPlayItemLayer.h \
    Classes/GPlayGardenData.h \
    Classes/GPlayFlowerPot.h \
    Classes/GPlayFlowerData.h \
    Classes/GPlayFlower.h \
    Classes/GPlayFingerTrace.h \
    Classes/GPauseLayer.h \
    Classes/GNumberLayer.h \
    Classes/GMainMenuScene.h \
    Classes/GMainMenuLayer.h \
    Classes/GLevelSelectScene.h \
    Classes/GLevelSelectLayer.h \
    Classes/GLevelInfoLayer.h \
    Classes/GLevelConfig.h \
    Classes/GGardenLayer.h \
    Classes/GGameOverLayer.h \
    Classes/GFlowerPickUnit.h \
    Classes/Gdefine.h \
    Classes/GDataManage.h \
    Classes/GCompletedLayer.h \
    Classes/GChallengeLayer.h \
    Classes/GardenAppDelegate.h \
    Classes/HelloWorldScene.h \
    Classes/GLayout.h \
    Classes/GSound.h \
    GParticles.h \
    GDefine.h \
    GPanelLayer.h \
    GPanelData.h \
    GParticlesData.h \
    GBeatSequence.h \
    Classes/GPlayerRecordLayer.h \
    Classes/GLevelDayLayer.h \
    Classes/GKindConfig.h \
    Classes/GCustomProjection.h \
    Classes/GChallengeConfig.h \
    Classes/GCentipeSegment.h \
    Classes/GCentipe.h \
    Classes/GBossCentipe.h

INCLUDEPATH += cocos2dx

FORMS    += mainwindow.ui
