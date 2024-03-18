/******************************************************************************
 * The MIT License (MIT)
 *
 * Copyright (c) 2019-2021 Baldur Karlsson
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 ******************************************************************************/

#include "renderdoccmd.h"
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <dlfcn.h>
#include <locale.h>
#include <string.h>
#include <unistd.h>
#include <string>

#include <android_native_app_glue.h>
#include <jni.h>

#include <android/log.h>
#define ANDROID_LOG(...) __android_log_print(ANDROID_LOG_INFO, "uwaclient", __VA_ARGS__);

byte *logo_data = nullptr;
unsigned logo_width = 0;
unsigned logo_height = 0;

extern "C" JNIEXPORT void JNICALL 
Java_com_uwa_uwaclient_arm32_Loader_updateLogoData(JNIEnv *env, jobject obj, jbyteArray data, jint width, jint height)
{
  jbyte *buffer = env->GetByteArrayElements(data, NULL);
  // ����logo��ͼ����
  jsize len = env->GetArrayLength(data);
  logo_data = new byte[len];
  memcpy(logo_data, buffer, len);
  logo_width = width;
  logo_height = height;

  ANDROID_LOG("arm32_updateLogoData : %i x %i", logo_width, logo_height);

  // �ͷ��ֽ�������ڴ�
  env->ReleaseByteArrayElements(data, buffer, JNI_ABORT);
}

extern "C" JNIEXPORT void JNICALL 
Java_com_uwa_uwaclient_arm64_Loader_updateLogoData(JNIEnv *env, jobject obj, jbyteArray data, jint width, jint height)
{
  jbyte *buffer = env->GetByteArrayElements(data, NULL);
  // ����logo��ͼ����
  jsize len = env->GetArrayLength(data);
  logo_data = new byte[len];
  memcpy(logo_data, buffer, len);
  logo_width = width;
  logo_height = height;

  ANDROID_LOG("arm32_updateLogoData : %i x %i", logo_width, logo_height);

  // �ͷ��ֽ�������ڴ�
  env->ReleaseByteArrayElements(data, buffer, JNI_ABORT);
}

struct android_app *android_state;
pthread_t cmdthread_handle = 0;

struct PThreadLock
{
  PThreadLock(const char *n) : name(n)
  {
    ANDROID_LOG("Creating lock %s", name);
    pthread_mutexattr_init(&mutexattr);
    pthread_mutexattr_settype(&mutexattr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&mutex, &mutexattr);
  }
  ~PThreadLock()
  {
    ANDROID_LOG("Destroying lock %s", name);
    pthread_mutex_destroy(&mutex);
    pthread_mutexattr_destroy(&mutexattr);
  }
  bool trylock() { return pthread_mutex_trylock(&mutex) == 0; };
  void lock() { pthread_mutex_lock(&mutex); }
  void unlock() { pthread_mutex_unlock(&mutex); }
private:
  const char *name;
  pthread_mutex_t mutex;
  pthread_mutexattr_t mutexattr;
};

PThreadLock m_DrawLock("m_DrawLock"), m_CmdLock("m_CmdLock");

void Daemonise()
{
}

void DisplayGenericSplash()
{
  ANDROID_LOG("Trying to splash");
  // if something else is drawing and holding the lock, then bail
  if(!m_DrawLock.trylock())
    return;
  ANDROID_LOG("Doing a splash");

  // since we're not pumping this continually and we only draw when we need to, we can just do the
  // full initialisation and teardown every time. This means we don't have to pay attention to
  // whether something else needs to create a context on the window - we just do it
  // opportunistically when we can hold the draw lock.
  // This only takes about 30ms anyway, so it's still technically realtime, right!?

  // fetch everything dynamically. We can't link against libEGL otherwise it messes with the hooking
  // in the main library. Just declare local function pointers with the real names
  void *libEGL = dlopen("libEGL.so", RTLD_NOW);

#define DLSYM_GET(name) decltype(&::name) name = (decltype(&::name))dlsym(libEGL, #name);
#define GPA_GET(name) decltype(&::name) name = (decltype(&::name))eglGetProcAddress(#name);

  DLSYM_GET(eglBindAPI);
  DLSYM_GET(eglGetDisplay);
  DLSYM_GET(eglInitialize);
  DLSYM_GET(eglGetError);
  DLSYM_GET(eglChooseConfig);
  DLSYM_GET(eglCreateContext);
  DLSYM_GET(eglCreateWindowSurface);
  DLSYM_GET(eglMakeCurrent);
  DLSYM_GET(eglDestroySurface);
  DLSYM_GET(eglDestroyContext);
  DLSYM_GET(eglGetProcAddress);
  DLSYM_GET(eglSwapBuffers);
  DLSYM_GET(eglTerminate);

  GPA_GET(glCreateShader);
  GPA_GET(glShaderSource);
  GPA_GET(glCompileShader);
  GPA_GET(glCreateProgram);
  GPA_GET(glAttachShader);
  GPA_GET(glLinkProgram);
  GPA_GET(glGetUniformLocation);
  GPA_GET(glUniform1i);
  GPA_GET(glUniform2f);
  GPA_GET(glUseProgram);
  GPA_GET(glVertexAttribPointer);
  GPA_GET(glEnableVertexAttribArray);
  GPA_GET(glDrawArrays);
  GPA_GET(glGetShaderiv);
  GPA_GET(glGetShaderInfoLog);
  GPA_GET(glGetProgramiv);
  GPA_GET(glGetProgramInfoLog);
  GPA_GET(glGenTextures);
  GPA_GET(glBindTexture);
  GPA_GET(glTexImage2D);
  GPA_GET(glTexParameteri);
  GPA_GET(glActiveTexture);

  eglBindAPI(EGL_OPENGL_ES_API);

  EGLDisplay eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);

  ANativeWindow *previewWindow = android_state->window;

  if(eglDisplay && previewWindow)
  {
    int major = 0, minor = 0;
    EGLBoolean initialised = eglInitialize(eglDisplay, &major, &minor);

    if(initialised && major >= 1)
    {
      const EGLint configAttribs[] = {EGL_RED_SIZE,
                                      8,
                                      EGL_GREEN_SIZE,
                                      8,
                                      EGL_BLUE_SIZE,
                                      8,
                                      EGL_SURFACE_TYPE,
                                      EGL_WINDOW_BIT,
                                      EGL_COLOR_BUFFER_TYPE,
                                      EGL_RGB_BUFFER,
                                      EGL_RENDERABLE_TYPE,
                                      EGL_OPENGL_ES2_BIT,
                                      EGL_NONE};

      EGLint numConfigs;
      EGLConfig config;
      if(eglChooseConfig(eglDisplay, configAttribs, &config, 1, &numConfigs))
      {
        // we only need GLES 2 for this
        static const EGLint ctxAttribs[] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE};

        EGLContext ctx = eglCreateContext(eglDisplay, config, NULL, ctxAttribs);
        if(ctx)
        {
          EGLSurface surface = eglCreateWindowSurface(eglDisplay, config, previewWindow, NULL);

          if(surface)
          {
            eglMakeCurrent(eglDisplay, surface, surface, ctx);

            unsigned int texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            if(logo_data != NULL)
            {
              glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, logo_width, logo_height, 0, GL_RGBA,
                           GL_UNSIGNED_BYTE, logo_data);
            }
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture);

            // simple pass through shader for the fullscreen triangle verts
            const char *vertex =
                "attribute vec2 pos;\n"
                "void main() { gl_Position = vec4(pos, 0.5, 1.0); }";

            const char *fragment = R"(
                precision highp float;
                uniform vec2 iResolution;
                uniform sampler2D tex;
                void main()
                {
                    vec2 uv = gl_FragCoord.xy / iResolution.xy;
                    vec4 finalcol = texture2D(tex, vec2(uv.x, 1.0 - uv.y));
	                gl_FragColor = finalcol;
                })";

            // compile the shaders and link into a program
            GLuint vs = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vs, 1, &vertex, NULL);
            glCompileShader(vs);

            GLint status = 0;
            char buffer[1025] = {0};
            glGetShaderiv(vs, GL_COMPILE_STATUS, &status);
            if(status == 0)
            {
              glGetShaderInfoLog(vs, 1024, NULL, buffer);
              ANDROID_LOG("VS error: %s", buffer);
            }

            GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fs, 1, &fragment, NULL);
            glCompileShader(fs);

            status = 0;
            glGetShaderiv(fs, GL_COMPILE_STATUS, &status);
            if(status == 0)
            {
              glGetShaderInfoLog(fs, 1024, NULL, buffer);
              ANDROID_LOG("FS error: %s", buffer);
            }

            GLuint prog = glCreateProgram();
            glAttachShader(prog, vs);
            glAttachShader(prog, fs);
            glLinkProgram(prog);

            status = 0;
            glGetProgramiv(prog, GL_LINK_STATUS, &status);
            if(status == 0)
            {
              glGetProgramInfoLog(prog, 1024, NULL, buffer);
              ANDROID_LOG("Program Error: %s", buffer);
            }

            glUseProgram(prog);
            GLuint loc = glGetUniformLocation(prog, "iResolution");
            glUniform2f(loc, float(ANativeWindow_getWidth(previewWindow)),
                        float(ANativeWindow_getHeight(previewWindow)));
            glUniform1i(glGetUniformLocation(prog, "tex"), 0);

            // fullscreen triangle
            float verts[] = {
                -1.0f, -1.0f,    // vertex 0
                3.0f,  -1.0f,    // vertex 1
                -1.0f, 3.0f,     // vertex 2
            };

            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, verts);
            glEnableVertexAttribArray(0);

            glDrawArrays(GL_TRIANGLES, 0, 3);

            eglSwapBuffers(eglDisplay, surface);
          }
          else
          {
            ANDROID_LOG("failed making surface: %x", eglGetError());
          }

          eglMakeCurrent(eglDisplay, 0L, 0L, NULL);
          eglDestroyContext(eglDisplay, ctx);
          eglDestroySurface(eglDisplay, surface);
        }
        else
        {
          ANDROID_LOG("failed making context: %x", eglGetError());
        }
      }
      else
      {
        ANDROID_LOG("failed choosing config");
      }

      eglTerminate(eglDisplay);
    }
    else
    {
      ANDROID_LOG("failed to initialise EGL");
    }
  }

  m_DrawLock.unlock();
  ANDROID_LOG("Done splashing");
}

WindowingData DisplayRemoteServerPreview(bool active, const rdcarray<WindowingSystem> &systems)
{
  static bool wasActive = false;

  // detect when the preview starts or stops
  if(wasActive != active)
  {
    wasActive = active;

    // if we're opening it, aquire the draw lock, otherwise release it.
    if(active)
    {
      ANDROID_LOG("Locking for preview");
      m_DrawLock.lock();
    }
    else
    {
      m_DrawLock.unlock();
      ANDROID_LOG("Unlocking from preview");

      // when we release it, re-draw the splash
      DisplayGenericSplash();
    }
  }

  WindowingData ret = {WindowingSystem::Unknown};

  if(android_state->window)
    ret = CreateAndroidWindowingData(android_state->window);

  return ret;
}

void DisplayRendererPreview(IReplayController *renderer, TextureDisplay &displayCfg, uint32_t width,
                            uint32_t height, uint32_t numLoops)
{
  ANativeWindow *connectionScreenWindow = android_state->window;

  m_DrawLock.lock();

  IReplayOutput *out = renderer->CreateOutput(CreateAndroidWindowingData(connectionScreenWindow),
                                              ReplayOutputType::Texture);

  out->SetTextureDisplay(displayCfg);

  if(numLoops == 0)
    numLoops = 100;

  for(uint32_t i = 0; i < numLoops; i++)
  {
    renderer->SetFrameEvent(10000000, true);

    ANDROID_LOG("Frame %u", i);
    out->Display();

    usleep(100000);
  }

  m_DrawLock.unlock();
}

// Returns the renderdoccmd arguments passed via am start
// Examples: am start ... -e renderdoccmd "remoteserver"
// -e renderdoccmd "replay /sdcard/capture.capture"
std::vector<std::string> getRenderdoccmdArgs()
{
  JNIEnv *env;
  android_state->activity->vm->AttachCurrentThread(&env, 0);

  jobject me = android_state->activity->clazz;

  jclass acl = env->GetObjectClass(me);    // class pointer of NativeActivity
  jmethodID giid = env->GetMethodID(acl, "getIntent", "()Landroid/content/Intent;");
  jobject intent = env->CallObjectMethod(me, giid);    // Got our intent

  jclass icl = env->GetObjectClass(intent);    // class pointer of Intent
  jmethodID gseid =
      env->GetMethodID(icl, "getStringExtra", "(Ljava/lang/String;)Ljava/lang/String;");

  jstring jsParam1 =
      (jstring)env->CallObjectMethod(intent, gseid, env->NewStringUTF("uwaclient"));

  std::vector<std::string> ret;
  if(jsParam1)    // Check if arg value found
  {
    ret.push_back("uwaclient");
    const char *param1 = env->GetStringUTFChars(jsParam1, 0);
    std::istringstream iss(param1);
    while(iss)
    {
      std::string sub;
      iss >> sub;
      ret.push_back(sub);
    }
  }
  android_state->activity->vm->DetachCurrentThread();

  return ret;
}

void *cmdthread(void *)
{
  std::vector<std::string> args = getRenderdoccmdArgs();
  if(args.size())
  {
    ANDROID_LOG("Entering cmd thread");
    m_CmdLock.lock();
    GlobalEnvironment env;
    renderdoccmd(env, args);
    m_CmdLock.unlock();
    ANDROID_LOG("Exiting cmd thread");
  }

  // activity is done and should be closed
  ANativeActivity_finish(android_state->activity);

  return NULL;
}

void handle_cmd(android_app *app, int32_t cmd)
{
  ANDROID_LOG("handle_cmd(%i)", cmd);
  switch(cmd)
  {
    case APP_CMD_INIT_WINDOW:
    {
      ANDROID_LOG("APP_CMD_INIT_WINDOW");
      // if we already have a thread handle, see if it's still running
      if(cmdthread_handle != 0)
      {
        ANDROID_LOG("thread handle exists");
        // if the thread isn't running anymore, we can acquire m_CmdLock. If so, we need to join the
        // thread and start afresh. If we can't acquire the lock, the thread is still running so we
        // leave it alone.
        if(m_CmdLock.trylock())
        {
          ANDROID_LOG("thread is dead, reaping");
          m_CmdLock.unlock();
          // safe to join here, thread will terminate soon if it hasn't already
          pthread_join(cmdthread_handle, NULL);
          cmdthread_handle = 0;
        }
      }

      // if we don't have a command thread, start one.
      if(cmdthread_handle == 0)
      {
        ANDROID_LOG("spawning command thread");
        pthread_create(&cmdthread_handle, NULL, cmdthread, NULL);
      }

      DisplayGenericSplash();
      break;
    }
    case APP_CMD_WINDOW_REDRAW_NEEDED:
    case APP_CMD_GAINED_FOCUS:
    case APP_CMD_LOST_FOCUS:
    {
      ANDROID_LOG("doing misc splash");
      DisplayGenericSplash();
      break;
    }
  }
}

void android_main(struct android_app *state)
{
  android_state = state;
  android_state->onAppCmd = handle_cmd;

  ANDROID_LOG("android_main android_state->window: %p", android_state->window);

  // Used to poll the events in the main loop
  int events;
  android_poll_source *source;
  do
  {
    if(ALooper_pollAll(1, nullptr, &events, (void **)&source) >= 0)
    {
      if(source != NULL)
        source->process(android_state, source);
    }
  } while(android_state->destroyRequested == 0);

  ANDROID_LOG("android_main exiting");
}
