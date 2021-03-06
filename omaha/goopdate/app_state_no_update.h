// Copyright 2009 Google Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// ========================================================================

#ifndef OMAHA_GOOPDATE_APP_STATE_NO_UPDATE_H_
#define OMAHA_GOOPDATE_APP_STATE_NO_UPDATE_H_

#include "base/basictypes.h"
#include "omaha/goopdate/app_state.h"

namespace omaha {

namespace fsm {

class AppStateNoUpdate : public AppState {
 public:
  AppStateNoUpdate();
  virtual ~AppStateNoUpdate() {}

  virtual const PingEvent* CreatePingEvent(App* app,
                                           CurrentState previous_state) const;

  // These calls are legal in this state but do nothing. This can occur when
  // no update is available for this app but updates are available for other
  // apps in the bundle.
  virtual void QueueDownload(App* app);
  virtual void QueueDownloadOrInstall(App* app);
  virtual void Download(App* app, DownloadManagerInterface* download_manager);
  virtual void QueueInstall(App* app);
  virtual void Install(App* app, InstallManagerInterface* install_manager);

  virtual void Cancel(App* app);
  virtual void Error(App* app,
                     const ErrorContext& error_context,
                     const CString& message);
 private:
  DISALLOW_COPY_AND_ASSIGN(AppStateNoUpdate);
};

}  // namespace fsm

}  // namespace omaha

#endif  // OMAHA_GOOPDATE_APP_STATE_NO_UPDATE_H_
