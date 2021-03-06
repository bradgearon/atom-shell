// Copyright (c) 2013 GitHub, Inc. All rights reserved.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

#include "atom/browser/api/atom_api_auto_updater.h"

#include "base/time/time.h"
#include "base/values.h"
#include "atom/browser/auto_updater.h"
#include "atom/browser/browser.h"
#include "native_mate/dictionary.h"
#include "native_mate/object_template_builder.h"

#include "atom/common/node_includes.h"

namespace atom {

namespace api {

AutoUpdater::AutoUpdater() {
  auto_updater::AutoUpdater::SetDelegate(this);
}

AutoUpdater::~AutoUpdater() {
  auto_updater::AutoUpdater::SetDelegate(NULL);
}

void AutoUpdater::OnError(const std::string& error) {
  base::ListValue args;
  args.AppendString(error);
  Emit("error", args);
}

void AutoUpdater::OnCheckingForUpdate() {
  Emit("checking-for-update");
}

void AutoUpdater::OnUpdateAvailable() {
  Emit("update-available");
}

void AutoUpdater::OnUpdateNotAvailable() {
  Emit("update-not-available");
}

void AutoUpdater::OnUpdateDownloaded(const std::string& release_notes,
                                     const std::string& release_name,
                                     const base::Time& release_date,
                                     const std::string& update_url,
                                     const base::Closure& quit_and_install) {
  quit_and_install_ = quit_and_install;

  base::ListValue args;
  args.AppendString(release_notes);
  args.AppendString(release_name);
  args.AppendDouble(release_date.ToJsTime());
  args.AppendString(update_url);
  Emit("update-downloaded-raw", args);
}

mate::ObjectTemplateBuilder AutoUpdater::GetObjectTemplateBuilder(
    v8::Isolate* isolate) {
  return mate::ObjectTemplateBuilder(isolate)
      .SetMethod("setFeedUrl", &auto_updater::AutoUpdater::SetFeedURL)
      .SetMethod("checkForUpdates", &auto_updater::AutoUpdater::CheckForUpdates)
      .SetMethod("_quitAndInstall", &AutoUpdater::QuitAndInstall);
}

void AutoUpdater::QuitAndInstall() {
  if (quit_and_install_.is_null())
    Browser::Get()->Shutdown();
  else
    quit_and_install_.Run();
}

// static
mate::Handle<AutoUpdater> AutoUpdater::Create(v8::Isolate* isolate) {
  return CreateHandle(isolate, new AutoUpdater);
}

}  // namespace api

}  // namespace atom


namespace {

void Initialize(v8::Handle<v8::Object> exports) {
  v8::Isolate* isolate = v8::Isolate::GetCurrent();
  mate::Dictionary dict(isolate, exports);
  dict.Set("autoUpdater", atom::api::AutoUpdater::Create(isolate));
}

}  // namespace

NODE_MODULE(atom_browser_auto_updater, Initialize)
