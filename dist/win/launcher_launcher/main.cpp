/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
** 
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
** 
** In addition, as a special exception, Nokia gives you certain
** additional rights. These rights are described in the Nokia Qt LGPL
** Exception version 1.0, included in the file LGPL_EXCEPTION.txt in this
** package.
** 
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
** 
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
** $END_LICENSE$

**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#define _CRT_SECURE_NO_DEPRECATE

#include <windows.h>
#include <jni.h>

typedef jint (JNICALL *Proc_JNI_CreateJavaVM)(JavaVM **, void **, void *);

bool non_unicode = false;
#define QT_WA(uni, ansi) if (!non_unicode) { uni } else { ansi }
#define QT_WA_INLINE(uni, ansi) (non_unicode ? ansi : uni)
#define BUFFER_SIZE 32767

wchar_t path_w[BUFFER_SIZE + MAX_PATH];
char    path_a[BUFFER_SIZE + MAX_PATH];
size_t  len;

static void clean_up()
{
    QT_WA(
    {
        SetEnvironmentVariableW(L"PATH", path_w + len);
    }, {
        SetEnvironmentVariableA( "PATH", path_a + len);
    });
}

static char *clearExceptionAndCopyString(const char *stringToCopy)
{
    char *newString = new char[strlen(stringToCopy) + 1];
    if (newString == 0)
        return 0;

    strcpy(newString, stringToCopy);

    return newString;
}

static char *getExceptionStackTrace(JNIEnv *jni_env)
{
    if (jni_env == 0)
        return 0;

    jthrowable exception = jni_env->ExceptionOccurred();
    if (exception == 0)
        return 0;

    jni_env->ExceptionClear();

    jclass stringWriterClass = jni_env->FindClass("java/io/StringWriter");
    if (stringWriterClass == 0)
        return clearExceptionAndCopyString("Cannot get stack trace due to missing java.io.StringWriter class.");

    jmethodID stringWriterConstructor = jni_env->GetMethodID(stringWriterClass, "<init>", "()V");
    if (stringWriterConstructor == 0)
        return clearExceptionAndCopyString("Cannot get stack trace due to missing StringWriter() constructor.");

    jobject stringWriter = jni_env->NewObject(stringWriterClass, stringWriterConstructor);
    if (stringWriter == 0)
        return clearExceptionAndCopyString("Cannot get stack trace because of failure to create StringWriter object.");

    jclass printWriterClass = jni_env->FindClass("java/io/PrintWriter");
    if (printWriterClass == 0)
        return clearExceptionAndCopyString("Cannot get stack trace due to missing java.io.PrintWriter class.");

    jmethodID printWriterConstructor = jni_env->GetMethodID(printWriterClass, "<init>", "(Ljava/io/Writer;)V");
    if (printWriterConstructor == 0)
        return clearExceptionAndCopyString("Cannot get stack trace due to missing PrintWriter(java.io.Writer) constructor.");

    jobject printWriter = jni_env->NewObject(printWriterClass, printWriterConstructor, stringWriter);
    if (printWriter == 0)
        return clearExceptionAndCopyString("Cannot get stack trace because of failure to create PrintWriter object.");

    jclass throwableClass = jni_env->FindClass("java/lang/Throwable");
    if (throwableClass == 0)
        return clearExceptionAndCopyString("Cannot get stack trace due to missing java.lang.Throwable class.");

    jmethodID printStackTraceMethod = jni_env->GetMethodID(throwableClass, "printStackTrace", "(Ljava/io/PrintWriter;)V");
    if (printStackTraceMethod == 0)
        return clearExceptionAndCopyString("Cannot get stack trace due to missing Throwable.printStackTrace(java.io.PrintWriter) method.");

    jni_env->CallVoidMethod(exception, printStackTraceMethod, printWriter);
    if (jni_env->ExceptionOccurred())
        return clearExceptionAndCopyString("Exception occurred while retrieving exception stack trace.");

    jmethodID stringWriterToStringMethod = jni_env->GetMethodID(stringWriterClass, "toString", "()Ljava/lang/String;");
    if (stringWriterToStringMethod == 0)
        return clearExceptionAndCopyString("Cannot get stack trace due to missing StringWriter.toString() method.");

    jstring string = reinterpret_cast<jstring>(jni_env->CallObjectMethod(stringWriter, stringWriterToStringMethod));
    if (string == 0)
        return clearExceptionAndCopyString("Cannot get stack trace because of failure to retrieve string from StringWriter object.");

    int stringLength = jni_env->GetStringLength(string);
    if (stringLength == 0)
        return clearExceptionAndCopyString("No exception stack trace available.");

    char *cString = new char[stringLength + 1];
    if (cString == 0)
        return clearExceptionAndCopyString("Not enough memory to get exception stack trace.");

    jni_env->GetStringUTFRegion(string, 0, stringLength, cString);
    if (jni_env->ExceptionOccurred())
        return clearExceptionAndCopyString("Exception occurred while converting exception stack trace to C string.");

    return cString;
}

static int bug_out(const char *title, JNIEnv *jni_env)
{
    clean_up();

    char *exceptionStackTrace = getExceptionStackTrace(jni_env);

    const char *errorMessage =
        "There was a problem running the Qt Jambi demos and\n"
        "examples. Please make sure that you have Java 1.5.0\n"
        "installed and that java.exe is available in the PATH\n"
        "environment. For further debugging you can try adding\n"
        "all bundled .jar files to your classpath and running\n"
        "the org.qtjambi.launcher.Launcher class.\n"
        "\n"
        "The specific error was:\n"
        "  %s\n"
        "%s\n";

    const char *doYouWantToSeeTheStackTrace = "Do you want to see the Java exception stack trace?";

    char *tmp = new char[strlen(errorMessage) + strlen(title) + strlen(doYouWantToSeeTheStackTrace)];
    sprintf(tmp, errorMessage, title, exceptionStackTrace != 0 ? doYouWantToSeeTheStackTrace : "");

    int aok = MessageBoxA(NULL,
        tmp,
        "Error running Qt Jambi examples",
        exceptionStackTrace != 0 ? MB_YESNO : MB_OK);

    if (exceptionStackTrace != 0 && aok == IDYES) {
        MessageBoxA(NULL,
                    exceptionStackTrace,
                    "Exception caught by Java when running org.qtjambi.launcher.Launcher",
                    MB_OK);
        printf(exceptionStackTrace);
    }

    delete exceptionStackTrace;
    delete tmp;

    return 1;
}

static HMODULE search_relative_position_for_jvm_dll(void *dir, void *relative_position)
{
    size_t dir_len = QT_WA_INLINE(wcslen((wchar_t *)dir), strlen((char *)dir));

    // Skip slash in end of dir name
    if (QT_WA_INLINE( (((wchar_t *)dir)[dir_len-1] == L'\\'),
                      (((char *)dir)[dir_len-1] == '\\'))) {
        dir_len --;
    }

    size_t relative_position_len = QT_WA_INLINE(wcslen((wchar_t *)relative_position), strlen((char *)relative_position));
    if (dir_len + relative_position_len > MAX_PATH)
        return 0;

    QT_WA(
    {
        wchar_t longer_path[MAX_PATH];
        wcsncpy(longer_path, (wchar_t *)dir, dir_len);
        wcsncpy(longer_path + dir_len, (wchar_t *)relative_position, relative_position_len);
        longer_path[dir_len + relative_position_len] = 0;

        return LoadLibraryW(longer_path);
    }, {
        char longer_path[MAX_PATH];
        strncpy(longer_path, (char *)dir, dir_len);
        strncpy(longer_path + dir_len, (char *)relative_position, relative_position_len);
        longer_path[dir_len + relative_position_len] = 0;

        return LoadLibraryA(longer_path);
    });
}

static HMODULE search_dir_for_jvm_dll(void *dir)
{
    WIN32_FIND_DATAA data_a;
    WIN32_FIND_DATAW data_w;
    void *data = QT_WA_INLINE((void *)&data_w, (void *)&data_a);

    size_t dir_len = QT_WA_INLINE(wcslen((wchar_t *)dir), strlen((char *)dir));

    void *java_exe_name = QT_WA_INLINE((void *)L"\\java.exe", (void *)"\\java.exe");
    size_t java_exe_len = QT_WA_INLINE(wcslen((wchar_t *)java_exe_name), strlen((char *)java_exe_name));

    if (dir_len + java_exe_len > MAX_PATH)
        return 0;

    // Skip slash in end of path
    if (QT_WA_INLINE( (((wchar_t *)dir)[dir_len-1] == L'\\'),
                      (((char *)dir)[dir_len-1] == '\\'))) {
        dir_len --;
    }

    HANDLE java_exe = INVALID_HANDLE_VALUE;
    QT_WA(
    {
        wchar_t java_search_path[MAX_PATH];
        wcsncpy(java_search_path, (wchar_t *)dir, dir_len);
        wcsncpy(java_search_path + dir_len, (wchar_t *)java_exe_name, java_exe_len);
        java_search_path[dir_len + java_exe_len] = 0;

        java_exe = FindFirstFileW(java_search_path, (LPWIN32_FIND_DATAW) data);
    }, {
        char java_search_path[MAX_PATH];
        strncpy(java_search_path, (char *)dir, dir_len);
        strncpy(java_search_path + dir_len, (char *)java_exe_name, java_exe_len);
        java_search_path[dir_len + java_exe_len] = 0;

        java_exe = FindFirstFileA(java_search_path, (LPWIN32_FIND_DATAA) data);
    });

    if (java_exe == INVALID_HANDLE_VALUE)
        return 0;


#define NUM_POSITIONS 2
    char *relative_positions_a[NUM_POSITIONS] = {
        "\\..\\jre\\bin\\client\\jvm.dll",
        "\\..\\jre\\bin\\server\\jvm.dll"
    };
    wchar_t *relative_positions_w[NUM_POSITIONS] = {
        L"\\..\\jre\\bin\\client\\jvm.dll",
        L"\\..\\jre\\bin\\server\\jvm.dll"
    };

    for (int i=0; i<NUM_POSITIONS; ++i) {
        HMODULE result = search_relative_position_for_jvm_dll(dir,
                            QT_WA_INLINE((void *)relative_positions_w[i], (void *)relative_positions_a[i]));
        if (result != 0)
            return result;
    }

    return 0;
}

static HMODULE search_path_for_jvm_dll_w()
{
    wchar_t static_buffer[BUFFER_SIZE];
    wchar_t *buffer = static_buffer;

    DWORD returned_sz = GetEnvironmentVariableW(L"PATH", buffer, BUFFER_SIZE);
    if (returned_sz == 0 || returned_sz > BUFFER_SIZE)
        return 0;

    int len = int(wcslen(buffer));

    // Split path
    for (wchar_t *ptr=buffer; *ptr != 0; ptr++) {
        if (*ptr == ';')
            *ptr = 0;
    }

    // Check each dir in path for java.exe and jvm.dll
    while (len > 0) {
        size_t buffer_len = wcslen(buffer);
        if (buffer_len > 0) {
            HMODULE result = search_dir_for_jvm_dll((void *)buffer);
            if (result != 0)
                return result;
        }

        len -= buffer_len + 1;
        buffer += buffer_len + 1;
    }

    return 0;
}

static HMODULE search_path_for_jvm_dll_a()
{
    char static_buffer[BUFFER_SIZE];
    char *buffer = static_buffer;

    DWORD returned_sz = GetEnvironmentVariableA("PATH", buffer, BUFFER_SIZE);
    if (returned_sz == 0 || returned_sz > BUFFER_SIZE)
        return 0;

    int len = int(strlen(buffer));

    // Split path
    for (char *ptr=buffer; *ptr != 0; ptr++) {
        if (*ptr == ';')
            *ptr = 0;
    }

    // Check each dir in path for java.exe and jvm.dll
    while (len > 0) {
        size_t buffer_len = strlen(buffer);
        if (buffer_len > 0) {
            HMODULE result = search_dir_for_jvm_dll((void *)buffer);
            if (result != 0)
                return result;
        }

        len -= buffer_len + 1;
        buffer += buffer_len + 1;
    }

    return 0;
}

static bool setup_path_w(char *dest, const wchar_t *extra, const wchar_t *environment_var)
{
    wchar_t tmp[MAX_PATH + BUFFER_SIZE];

    size_t extra_len = wcslen(extra);
    if (extra_len > MAX_PATH + BUFFER_SIZE)
        return false;

    wcsncpy(tmp, extra, extra_len);

    DWORD remaining_space = DWORD(MAX_PATH + BUFFER_SIZE - extra_len);
    DWORD sz = GetEnvironmentVariableW(environment_var, tmp + extra_len, remaining_space);
    if (sz > remaining_space)
        return false;

    // Convert to UTF-8
    if (WideCharToMultiByte(CP_UTF8, 0, tmp, -1, dest, MAX_PATH + BUFFER_SIZE, NULL, NULL) == 0)
        return false;
    dest[extra_len + sz] = '\0';

    return true;
}

static bool setup_path_a(char *dest, const char *extra, const char *environment_var)
{
    size_t extra_len = strlen(extra);
    if (extra_len > MAX_PATH + BUFFER_SIZE)
        return false;

    strncpy(dest, extra, extra_len);

    DWORD remaining_space = DWORD(MAX_PATH + BUFFER_SIZE - extra_len);
    DWORD sz = GetEnvironmentVariableA(environment_var, dest + extra_len, remaining_space);
    if (sz > remaining_space)
        return false;
    dest[extra_len + sz] = '\0';

    return true;
}



static bool launch_launcher(JNIEnv *jni_env)
{
    const char *launcher = "org/qtjambi/launcher/Launcher";

    jclass launcher_class = jni_env->FindClass(launcher);

    jmethodID main_function = 0;
    if (launcher_class != 0)
        main_function = jni_env->GetStaticMethodID(launcher_class, "main", "([Ljava/lang/String;)V");
    else
        return bug_out("Cannot find org.qtjambi.launcher.Launcher", jni_env) != 0;

    if (main_function != 0)
        jni_env->CallStaticVoidMethod(launcher_class, main_function, jobject(0));
    else
        return bug_out("Cannot find main function in Launcher class", jni_env) != 0;

    // Fail on exception
    if (jni_env->ExceptionOccurred()) {
        jni_env->ExceptionClear();
        return bug_out("Exception in Launcher", jni_env) != 0;
    }

    return true;
}

enum QtMsgType { QtDebugMsg, QtWarningMsg, QtCriticalMsg, QtFatalMsg, QtSystemMsg = QtCriticalMsg };
typedef void (*QtMsgHandler)(QtMsgType, const char *);
typedef QtMsgHandler (*Proc_qInstallMsgHandler)(QtMsgHandler);

static void message_handler(QtMsgType, const char *str)
{
    FILE *f = fopen("QT_MESSAGE_OUTPUT.TXT", "a");
    if (f != 0) {
        fprintf(f, str);
        fprintf(f, "\n");
        fclose(f);
    }
}

static bool check_java_version(JNIEnv *env)
{
    jstring key = env->NewStringUTF("java.version");

    jclass clazz = env->FindClass("java/lang/System");
    if (env->ExceptionOccurred() || clazz == 0)
        return false;

    jmethodID methodId = env->GetStaticMethodID(clazz, "getProperty", "(Ljava/lang/String;)Ljava/lang/String;");
    if (env->ExceptionOccurred() || methodId == 0)
        return false;

    jstring prop = (jstring) env->CallStaticObjectMethod(clazz, methodId, key);
    if (env->ExceptionOccurred() || prop == 0)
        return false;

    jsize len = env->GetStringUTFLength(prop);
    if (env->ExceptionOccurred() || len < 3)
        return false;

    const char *chars = env->GetStringUTFChars(prop, 0);
    if (chars[0] == '1' && chars[2] < '5')
        return false;

    env->ReleaseStringUTFChars(prop, chars);

    return true;
}

static LONG find_best_registry_key_a(PHKEY key)
{
    char *sub_key = "SOFTWARE\\JAVASOFT\\JAVA RUNTIME ENVIRONMENT";
    char best_key[255];
    size_t best_len = 0;
    DWORD sz = 255;

    HKEY parent_key;
    LONG result = RegOpenKeyExA(HKEY_LOCAL_MACHINE, sub_key, NULL, KEY_READ, &parent_key);
    if (result != ERROR_SUCCESS)
        return result;

    // Enumerate all run times and try to find the one with the highest version
    int i=0;
    while (result == ERROR_SUCCESS) {
        FILETIME dummy_tm;
        char got_key[255];

        sz = 255;
        result = RegEnumKeyExA(parent_key, i++, got_key, &sz, NULL, NULL, NULL, &dummy_tm);

        if (result == ERROR_SUCCESS) {
            size_t len = strlen(got_key);
            if (best_len != 0) {
                size_t upper_limit = len > best_len ? best_len : len;
                for (size_t i=0; i<upper_limit; ++i) {
                    if (got_key[i] > best_key[i]) {
                        best_len = 0;
                        break ;
                    }
                }
            }

            if (best_len == 0) {
                best_len = len;
                strncpy(best_key, got_key, len);
                best_key[len] = 0;
            }
        }
    }

    if (best_len != 0) {
        result = RegOpenKeyExA(parent_key, best_key, NULL, KEY_READ, key);
        RegCloseKey(parent_key);
        return result;
    } else {
        return ERROR_SUCCESS + 1; // just anything that's different from ERROR_SUCCESS here to indicate failure
    }
}

static HMODULE search_registry_for_jvm_dll_a()
{
    char *runtime_value = "RUNTIMELIB";
    char *sub_key = "SOFTWARE\\JAVASOFT\\JAVA RUNTIME ENVIRONMENT\\1.5";

    HKEY key;

    // Default to version 1.5 if it is installed
    LONG result = RegOpenKeyExA(HKEY_LOCAL_MACHINE, sub_key, NULL, KEY_READ, &key);
    if (result != ERROR_SUCCESS)
        result = find_best_registry_key_a(&key);

    if (result != ERROR_SUCCESS)
        return 0;

    DWORD type;
    char path[MAX_PATH];
    DWORD sz = MAX_PATH * sizeof(char);
    result = RegQueryValueExA(key, runtime_value, NULL, &type, (LPBYTE) path, &sz);
    RegCloseKey(key);
    if (result != ERROR_SUCCESS || type != REG_SZ)
        return 0;

    return LoadLibraryA(path);
}

static LONG find_best_registry_key_w(PHKEY key)
{
    wchar_t *sub_key = L"SOFTWARE\\JAVASOFT\\JAVA RUNTIME ENVIRONMENT";
    wchar_t best_key[255];
    size_t best_len = 0;
    DWORD sz = 255;

    HKEY parent_key;
    LONG result = RegOpenKeyExW(HKEY_LOCAL_MACHINE, sub_key, NULL, KEY_READ, &parent_key);
    if (result != ERROR_SUCCESS)
        return result;

    // Enumerate all run times and try to find the one with the highest version
    int i=0;
    while (result == ERROR_SUCCESS) {
        FILETIME dummy_tm;
        wchar_t got_key[255];

        sz = 255;
        result = RegEnumKeyExW(parent_key, i++, got_key, &sz, NULL, NULL, NULL, &dummy_tm);

        if (result == ERROR_SUCCESS) {
            size_t len = wcslen(got_key);
            if (best_len != 0) {
                size_t upper_limit = len > best_len ? best_len : len;
                for (size_t i=0; i<upper_limit; ++i) {
                    if (got_key[i] > best_key[i]) {
                        best_len = 0;
                        break ;
                    }
                }
            }

            if (best_len == 0) {
                best_len = len;
                wcsncpy(best_key, got_key, len);
                best_key[len] = 0;
            }
        }
    }

    if (best_len != 0) {
        result = RegOpenKeyExW(parent_key, best_key, NULL, KEY_READ, key);
        RegCloseKey(parent_key);
        return result;
    } else {
        return ERROR_SUCCESS + 1; // just anything that's different from ERROR_SUCCESS here to indicate failure
    }
}

static HMODULE search_registry_for_jvm_dll_w()
{
    wchar_t *runtime_value = L"RUNTIMELIB";
    wchar_t *sub_key = L"SOFTWARE\\JAVASOFT\\JAVA RUNTIME ENVIRONMENT\\1.5";

    HKEY key;

    // Default to version 1.5 if it is installed
    LONG result = RegOpenKeyExW(HKEY_LOCAL_MACHINE, sub_key, NULL, KEY_READ, &key);
    if (result != ERROR_SUCCESS)
        result = find_best_registry_key_w(&key);

    if (result != ERROR_SUCCESS)
        return 0;

    DWORD type;
    wchar_t path[MAX_PATH];
    DWORD sz = MAX_PATH * sizeof(wchar_t);
    result = RegQueryValueExW(key, runtime_value, NULL, &type, (LPBYTE) path, &sz);
    RegCloseKey(key);
    if (result != ERROR_SUCCESS || type != REG_SZ)
        return 0;

    return LoadLibraryW(path);
}

bool decide_library_version(char *str)
{
    char name[] = "qtjambi-4.X.Y.jar";
    for (int build = 1; build < 9; ++build) {
        for (int minor = 4; minor<9; ++minor) {
            for (int patch = 0; patch < 9; ++patch) {
                name[10] = '0' + minor;
                name[12] = '0' + patch;
             //   name[15] = '0' + build;
                if (GetFileAttributesA(name) != 0xffffffff) {
                    str[2] = '0' + minor;
                    str[4] = '0' + patch;
              //      str[7] = '0' + build;
                    return true;
                }
            }
        }
    }
    return false;
}

#if defined(_DEBUG)
#  define NOPTIONS 2
#else
#  define NOPTIONS 1
#endif

#ifdef QT_CONSOLE_BUILD
int main(int, char**)
#else
int __stdcall WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
#endif
{
    JNIEnv *jni_env;
    JavaVM *vm;

    OSVERSIONINFO ove;
    ove.dwOSVersionInfoSize = sizeof(ove);
    GetVersionEx(&ove);
    non_unicode = ove.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS; // 95, 98, Me

    DWORD dw_sz;
    QT_WA(
    {
        const wchar_t *new_path = L".\\bin;";
        len = wcslen(new_path);
        wcsncpy(path_w, new_path, len);

        DWORD remaining_space = DWORD(BUFFER_SIZE - len);
        dw_sz = GetEnvironmentVariableW(L"PATH", path_w + len, remaining_space);
        if (dw_sz <= remaining_space) {
            path_w[len + dw_sz] = L'\0';
            SetEnvironmentVariableW(L"PATH", path_w);
        } else {
            dw_sz = 0;
        }
    }, {
        const char *new_path = ".\\bin;";
        len = strlen(new_path);
        strncpy(path_a, new_path, len);
        DWORD remaining_space = DWORD(BUFFER_SIZE - len);
        dw_sz = GetEnvironmentVariableA("PATH", path_a + len, remaining_space);
        if (dw_sz <= remaining_space){
            path_a[len + dw_sz] = '\0';
            SetEnvironmentVariableA("PATH", path_a);
        } else {
            dw_sz = 0;
        }
    });

    char version[] = "4.X.Y";
    if (!decide_library_version(version)) {
        bug_out("Failed to detect library versions...\n", 0);
    }

    // Set up VM options
    void *extra_class_path   = QT_WA_INLINE((void *)L"-Djava.class.path=qtjambi-$VER$.jar;qtjambi-examples-$VER$.jar;qtjambi-util-$VER$.jar;qtjambi-win64-msvc2008x64-$VER$.jar;qtjambi-win32-msvc2008-$VER$.jar;",
                                            (void *)"-Djava.class.path=qtjambi-$VER$.jar;qtjambi-examples-$VER$.jar;qtjambi-util-$VER$.jar;qtjambi-win64-msvc2008x64-$VER$.jar;qtjambi-win32-msvc2008-$VER$.jar;");

    JavaVMInitArgs args;
    args.version = JNI_VERSION_1_4;
    args.ignoreUnrecognized = JNI_FALSE;
    args.nOptions = NOPTIONS;

    char new_class_path[BUFFER_SIZE + MAX_PATH];
    QT_WA(
    {
        if (!setup_path_w(new_class_path, (wchar_t *)extra_class_path,
                          L"CLASSPATH")) {
            return bug_out("Failed to set CLASSPATH environment", 0);
        }
    }, {
        if (!setup_path_a(new_class_path, (char *)extra_class_path,
                          "CLASSPATH")) {
            return bug_out("Failed to set CLASSPATH environment", 0);
        }
    });

    // search replace all $VERHAX$ with version
    char *pos = new_class_path;

    while (pos = strstr(pos, "$VER$")) {
        strncpy(pos, version, strlen(version));
    }

    JavaVMOption options[NOPTIONS];
    options[0].optionString = new_class_path;


#if defined(_DEBUG)
    options[1].optionString = "-Dorg.qtjambi.qt.debug";
#endif

    args.options = options;

    Proc_JNI_CreateJavaVM addr_JNI_CreateJavaVM = 0;
    HMODULE jvm_dll = QT_WA_INLINE(search_registry_for_jvm_dll_w(), search_registry_for_jvm_dll_a());

    if (jvm_dll == 0)
        LoadLibraryA("jvm.dll");
    if (jvm_dll == 0)
        jvm_dll = QT_WA_INLINE(search_path_for_jvm_dll_w(), search_path_for_jvm_dll_a());
    if (jvm_dll == 0)
        return bug_out("Failed to find jvm.dll", 0);

    addr_JNI_CreateJavaVM = (Proc_JNI_CreateJavaVM)GetProcAddress(jvm_dll, "JNI_CreateJavaVM");
    if (addr_JNI_CreateJavaVM == 0)
        return bug_out("Failed to find CreateJavaVM procedure", 0);

    {
        jint result = addr_JNI_CreateJavaVM(&vm, (void **)&jni_env, &args);
        if (result != 0 || vm == 0 || jni_env == 0)
            return bug_out("Failed to create Java Virtual Machine", 0);

        if (!check_java_version(jni_env))
            return bug_out("Wrong Java version detected", jni_env);

    }

    {
        bool success = launch_launcher(jni_env);
        vm->DestroyJavaVM();

        if (!success) {
            return 1;
        } else {
            clean_up();
            return 0;
        }
    }
}
