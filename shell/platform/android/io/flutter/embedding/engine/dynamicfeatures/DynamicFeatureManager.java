// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

package io.flutter.embedding.engine.dynamicfeatures;

// TODO: add links to external documentation on how to use split aot features.
/**
 * Basic interface that handles downloading and loading of dynamic features.
 *
 * <p>Flutter dynamic feature support is still in early developer preview and should
 * not be used in production apps yet.
 *
 * <p>The Flutter default implementation is PlayStoreDynamicFeatureManager.
 *
 * <p>DynamicFeatureManager handles the embedder/android level tasks of downloading, installing, and
 * loading dart deferred libraries. A typical code-flow begins with a dart call to loadLibrary() on
 * deferred imported library. See https://dart.dev/guides/language/language-tour#deferred-loading
 * This call retrieves a unique identifier called the loading unit id, which is assigned by
 * gen_snapshot during compilation. The loading unit id is passed down through the engine and invokes
 * downloadDynamicFeature. Once the feature module is downloaded, loadAssets and loadDartLibrary should
 * be invoked. loadDartLibrary should find shared library .so files for the engine to open. loadAssets
 * should typically ensure the new assets are available to the engine's asset manager by passing an
 * updated android AssetManager to the engine.
 *
 * <p>The loadAssets and loadDartLibrary methods are separated out because they may also be called
 * manually via platform channel messages. A full downloadDynamicFeature implementation should call
 * these two methods as needed.
 *
 * <p>A dynamic feature module is uniquely identified by a module name as defined in bundle_config.yaml.
 * Each feature module may contain one or more loading units, uniquely identified by the loading unit ID
 * and assets.
 */
public interface DynamicFeatureManager {
  /**
   * Request that the feature module be downloaded and installed.
   *
   * <p>This method is called by Flutter when loadLibrary() is called on a dart deferred library
   * which relies on this method to download the requested module under the hood. This method may
   * also be manually invoked via a system channel message.
   *
   * <p>This method begins the download and installation of the specified feature module. For example,
   * the Play Store dynamic delivery implementation uses SplitInstallManager to request the download of
   * the module. Download is not complete when this method returns. The download process should be
   * listened for and upon completion of download, listeners should invoke loadAssets and loadDartLibrary
   * to complete the dynamic feature load process.
   *
   * <p>Both parameters are not always necessary to identify which module to install. Asset-only modules
   * do not have an associated loadingUnitId. Instead, an invalid ID like -1 may be passed to download
   * only with moduleName. On the other hand, it can be possible to resolve the moduleName based
   * on the loadingUnitId. This resolution is done if moduleName is null. At least one of
   * loadingUnitId or moduleName must be valid or non-null.
   *
   * <p>Flutter will typically call this method in two ways. When invoked as part of a dart loadLibrary()
   * call, a valid loadingUnitId is passed in while the moduleName is null. In this case, this method is
   * responsible for figuring out what module the loadingUnitId corresponds to.
   *
   * <p>When invoked manually as part of loading an assets-only module, loadingUnitId is -1 (invalid)
   * and moduleName is supplied. Without a loadingUnitId, this method just downloads the module by name
   * and attempts to load assets via loadAssets.
   *
   * @param loadingUnitId The unique identifier associated with a dart deferred library. This id is
   *                      assigned by the compiler and can be seen for reference in bundle_config.yaml.
   *                      This ID is primarily used in loadDartLibrary to indicate to dart which
   *                      dart library is being loaded. Loading unit ids range from 0 to the number
   *                      existing loading units. Passing a negative loading unit id indicates
   *                      that no dart deferred library should be loaded after download completes.
   *                      This is the case when the dynamic feature module is an assets-only module.
   *                      If a negative loadingUnitId is passed, then moduleName must not be null.
   *                      Passing a loadingUnitId larger than the highest valid loading unit's id will
   *                      cause the dart loadLibrary() to complete with a failure.
   *
   * @param moduleName    The dynamic feature module name as defined in bundle_config.yaml. This may
   *                      be null if the dynamic feature to be loaded is associated with a
   *                      loading unit/deferred dart library. In this case, it is this method's
   *                      responsibility to map the loadingUnitId to its corresponding moduleName.
   *                      When loading asset-only or other dynamic features without an associated
   *                      dart deferred library, loading unit id should a negative value and moduleName
   *                      must be non-null.
   */
  public abstract void downloadDynamicFeature(int loadingUnitId, String moduleName);

  /**
   * Extract and load any assets and resources from the module for use by Flutter.
   *
   * <p>Assets shoud be loaded before the dart derferred library is loaded, as successful loading of
   * the dart loading unit indicates the dynamic feature is fully loaded.
   *
   * <p>Depending on the structure of the feature module, there may or may not be assets to extract.
   *
   * <p>If using the Play Store dynamic feature delivery, refresh the context via:
   * {@code context.createPackageContext(context.getPackageName(), 0);}
   * This returns a new context, from which an updated asset manager may be obtained and passed to
   * updateAssetManager in FlutterJNI. This process does not require loadingUnitId or moduleName,
   * however, the two parameters are still present for custom implementations that store assets
   * outside of Android's native system.
   *
   * @param loadingUnitId The unique identifier associated with a dart deferred library.
   *
   * @param moduleName    The dynamic feature module name as defined in bundle_config.yaml.
   */
  public abstract void loadAssets(int loadingUnitId, String moduleName);

  /**
   * Load the .so shared library file into the Dart VM.
   *
   * <p>When a feature module download is triggered via a dart loadLibrary() call on a deferred library
   * and the download completes, this method should be called to find the path .so library file and
   * passed to FlutterJNI.loadDartDeferredLibrary to be dlopen-ed.
   *
   * <p>Specifically, APKs distributed by android's app bundle format may vary by device and API number,
   * so FlutterJNI's loadDartDeferredLibrary accepts a list of search paths with can include paths
   * within APKs that have not been unpacked using the `path/to/apk.apk!path/inside/apk/lib.so` format.
   * Each search path will be attempted in order until a shared library is found. This allows for
   * the developer to avoid unpacking the apk zip.
   *
   * <p>Upon successful load of the dart library, the dart future from the originating loadLibary()
   * call completes and developers are able to use symbols and assets from the feature module.
   *
   * @param loadingUnitId The unique identifier associated with a dart deferred library. This id is
   *                      assigned by the compiler and can be seen for reference in bundle_config.yaml.
   *                      This ID is primarily used in loadDartLibrary to indicate to dart which
   *                      dart library is being loaded. Loading unit ids range from 0 to the number
   *                      existing loading units. Negative loading unit ids are considered
   *                      invalid and this method will result in a no-op.
   *
   * @param moduleName    The dynamic feature module name as defined in bundle_config.yaml. If using
   *                      Play Store dynamic feature delivery, this name corresponds to the root name
   *                      on the installed APKs in which to search for the desired shared library .so
   *                      file.
   */
  public abstract void loadDartLibrary(int loadingUnitId, String moduleName);

  /**
   * Uninstall the specified feature module.
   *
   * <p>Both parameters are not always necessary to identify which module to uninstall. Asset-only
   * modules do not have an associated loadingUnitId. Instead, an invalid ID like -1 may be passed to
   * download only with moduleName. On the other hand, it can be possible to resolve the moduleName based
   * on the loadingUnitId. This resolution is done if moduleName is null. At least one of
   * loadingUnitId or moduleName must be valid or non-null.
   */
  public abstract void uninstallFeature(int loadingUnitId, String moduleName);
}
