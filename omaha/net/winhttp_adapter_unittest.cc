// Copyright 2010 Google Inc.
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

#include <windows.h>
#include "base/scoped_ptr.h"
#include "omaha/net/winhttp_adapter.h"
#include "omaha/testing/unit_test.h"

namespace omaha {

// Tests null WinHttp handles are allowed for some WinHttp calls.
TEST(WinHttpAdapter, NullHandles) {
  WinHttpAdapter winhttp_adapter;
  EXPECT_HRESULT_SUCCEEDED(winhttp_adapter.Initialize());

  // Opens a session, connects, closes the connection handle, and then
  // tries to open a request for the null connection.
  scoped_ptr<HttpClient> http_client(CreateHttpClient());
  EXPECT_HRESULT_SUCCEEDED(http_client->Initialize());

  HINTERNET session_handle = NULL;
  EXPECT_HRESULT_SUCCEEDED(http_client->Open(NULL,
                                             WINHTTP_ACCESS_TYPE_NO_PROXY,
                                             WINHTTP_NO_PROXY_NAME,
                                             WINHTTP_NO_PROXY_BYPASS,
                                             WINHTTP_FLAG_ASYNC,
                                             &session_handle));
  EXPECT_NE(static_cast<HINTERNET>(NULL), session_handle);
  EXPECT_HRESULT_SUCCEEDED(winhttp_adapter.Connect(session_handle,
                                                   _T("127.0.0.1"),
                                                   80));
  winhttp_adapter.CloseHandles();
  EXPECT_EQ(HRESULT_FROM_WIN32(ERROR_INVALID_HANDLE),
            winhttp_adapter.OpenRequest(NULL,  // verb
                                        NULL,  // uri,
                                        NULL,  // version
                                        NULL,  // referrer
                                        NULL,  // accept_types
                                        0));   // flags

  // Null request handle.
  EXPECT_EQ(HRESULT_FROM_WIN32(ERROR_INVALID_HANDLE),
            winhttp_adapter.QueryDataAvailable(NULL));

  EXPECT_HRESULT_SUCCEEDED(http_client->Close(session_handle));
}

// Tests opening a request then closing it before any activity happens
// on the network. This tests that the callback is correctly set and the
// closing handle callback is handled.
TEST(WinHttpAdapter, OpenRequestClose) {
  WinHttpAdapter winhttp_adapter;
  EXPECT_HRESULT_SUCCEEDED(winhttp_adapter.Initialize());

  // Opens a session, connects, opens the request, then closes the handles.
  scoped_ptr<HttpClient> http_client(CreateHttpClient());
  EXPECT_HRESULT_SUCCEEDED(http_client->Initialize());

  HINTERNET session_handle = NULL;
  EXPECT_HRESULT_SUCCEEDED(http_client->Open(NULL,
                                             WINHTTP_ACCESS_TYPE_NO_PROXY,
                                             WINHTTP_NO_PROXY_NAME,
                                             WINHTTP_NO_PROXY_BYPASS,
                                             WINHTTP_FLAG_ASYNC,
                                             &session_handle));
  EXPECT_NE(static_cast<HINTERNET>(NULL), session_handle);
  EXPECT_HRESULT_SUCCEEDED(winhttp_adapter.Connect(session_handle,
                                                   _T("127.0.0.1"),
                                                   80));
  EXPECT_HRESULT_SUCCEEDED(
      winhttp_adapter.OpenRequest(NULL,  // verb
                                  NULL,  // uri,
                                  NULL,  // version
                                  NULL,  // referrer
                                  NULL,  // accept_types
                                  0));   // flags

  EXPECT_HRESULT_SUCCEEDED(http_client->Close(session_handle));
}

}  // namespace omaha
