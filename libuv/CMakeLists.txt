cmake_minimum_required(VERSION 2.8)
project(libuv C)

option(LIBUV_BUILD_TESTS "builds libuv tests and benchmarks if set to ON" OFF)

set(LIBUVDIR libuv)

include_directories(include src)
set(SOURCES
    include/uv.h
    include/tree.h
    include/uv-errno.h
    include/uv-threadpool.h
    include/uv-version.h
    src/fs-poll.c
    src/heap-inl.h
    src/inet.c
    src/queue.h
    src/threadpool.c
    src/uv-common.c
    src/uv-common.h
    src/version.c
    )

if(WIN32)
    add_definitions(-D_GNU_SOURCE)
    include_directories(src/win)
    set (SOURCES ${SOURCES}
        include/uv-win.h
        src/win/async.c
        src/win/atomicops-inl.h
        src/win/core.c
        src/win/dl.c
        src/win/error.c
        src/win/fs.c
        src/win/fs-event.c
        src/win/getaddrinfo.c
        src/win/getnameinfo.c
        src/win/handle.c
        src/win/handle-inl.h
        src/win/internal.h
        src/win/loop-watcher.c
        src/win/pipe.c
        src/win/thread.c
        src/win/poll.c
        src/win/process.c
        src/win/process-stdio.c
        src/win/req.c
        src/win/req-inl.h
        src/win/signal.c
        src/win/stream.c
        src/win/stream-inl.h
        src/win/tcp.c
        src/win/tty.c
        src/win/timer.c
        src/win/udp.c
        src/win/util.c
        src/win/winapi.c
        src/win/winapi.h
        src/win/winsock.c
        src/win/winsock.h    
        )

    add_definitions (-DWIN32_LEAN_AND_MEAN)
    add_library(uv ${SOURCES})
    target_link_libraries (uv advapi32.lib iphlpapi.lib psapi.lib userenv.lib shell32.lib ws2_32.lib)
else()
    add_definitions(-g --std=gnu89 -pedantic -Wall -Wextra -Wno-unused-parameter -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64)
    include_directories(src/unix)
    set(SOURCES ${SOURCES}
        include/uv-unix.h
        include/uv-linux.h
        include/uv-sunos.h
        include/uv-darwin.h
        include/uv-bsd.h
        include/uv-aix.h
        src/unix/async.c
        src/unix/atomic-ops.h
        src/unix/core.c
        src/unix/dl.c
        src/unix/fs.c
        src/unix/getaddrinfo.c
        src/unix/getnameinfo.c
        src/unix/internal.h
        src/unix/loop.c
        src/unix/loop-watcher.c
        src/unix/pipe.c
        src/unix/poll.c
        src/unix/process.c
        src/unix/signal.c
        src/unix/spinlock.h
        src/unix/stream.c
        src/unix/tcp.c
        src/unix/thread.c
        src/unix/timer.c
        src/unix/tty.c
        src/unix/udp.c
        )

    if (APPLE)
        add_definitions(-D_DARWIN_USE_64_BIT_INODE=1 -D_DARWIN_UNLIMITED_SELECT=1)
        set(SOURCES ${SOURCES}
            src/unix/proctitle.c
            src/unix/kqueue.c
            src/unix/darwin.c
            src/unix/fsevents.c
            src/unix/darwin-proctitle.c
            )

        #This is necessary to mute harmless warnings, as documented by https://github.com/libuv/libuv/issues/454
        set_source_files_properties(src/unix/stream.c PROPERTIES COMPILE_FLAGS -Wno-gnu-folding-constant)
    endif()

    if (NOT APPLE)
        add_definitions(-Wstrict-aliasing)
    endif()

    if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
        add_definitions(-D_GNU_SOURCE -D_POSIX_C_SOURCE=200112)
        set(SOURCES ${SOURCES}
            src/unix/proctitle.c
            src/unix/linux-core.c
            src/unix/linux-inotify.c
            src/unix/linux-syscalls.c
            src/unix/linux-syscalls.h
            )
    endif()

    if (${CMAKE_SYSTEM_NAME} MATCHES "SunOS")
        add_definitions(-D__EXTENSIONS__ -D_XOPEN_SOURCE=500)
        set(SOURCES ${SOURCES}
            src/unix/sunos.c
            )
    endif()

    if (${CMAKE_SYSTEM_NAME} MATCHES "AIX")
        add_definitions(-D_ALL_SOURCE -D_XOPEN_SOURCE=500 -D_LINUX_SOURCE_COMPAT)
        set (SOURCES ${SOURCES}
            src/unix/aic.c
            )
        find_library(PERFSTAT_LIB NAMES perfstat)
    endif()

    if (${CMAKE_SYSTEM_NAME} MATCHES "FreeBSD" OR ${CMAKE_SYSTEM_NAME} MATCHES "DragonFlyBSD")
        set (SOURCES ${SOURCES}
            src/unix/freebsd.c
            )
    endif()

    if (${CMAKE_SYSTEM_NAME} MATCHES "OpenBSD")
        set (SOURCES ${SOURCES}
            src/unix/openbsd.c
            )
    endif()

    if (${CMAKE_SYSTEM_NAME} MATCHES "NetBSD")
        set (SOURCES ${SOURCES}
            src/unix/netbsd.c
            )
    endif()

    if (${CMAKE_SYSTEM_NAME} MATCHES "FreeBSD" OR ${CMAKE_SYSTEM_NAME} MATCHES "DragonFlyBSD" OR
            ${CMAKE_SYSTEM_NAME} MATCHES "OpenBSD" OR ${CMAKE_SYSTEM_NAME} MATCHES "NetBSD")
        set (SOURCES ${SOURCES}
            src/unix/kqueue.c
            )
    endif()

    add_library(uv ${SOURCES})

    find_library(M_LIB NAMES m)
    find_package(Threads)
    target_link_libraries(uv ${CMAKE_THREAD_LIBS_INIT} ${M_LIB})

    if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
        find_library(DL_LIB NAMES dl)
        find_library(RT_LIB NAMES rt)
        target_link_libraries(uv ${DL_LIB} ${RT_LIB})
    endif()

    if (${CMAKE_SYSTEM_NAME} MATCHES "SunOS")
        find_library(KSTAT_LIB NAMES kstat)
        find_library(NSL_LIB NAMES nsl)
        find_library(SENDFILE_LIB NAMES sendfile)
        find_library(SOCKET_LIB NAMES socket)
        target_link_libraries(uv ${KSTAT_LIB} ${NSL_LIB} ${SENDFILE_LIB} ${SOCKET_LIB})
    endif()

    if (${CMAKE_SYSTEM_NAME} MATCHES "AIX")
        find_library(PERFSTAT_LIB NAMES perfstat)
        target_link_libraries(uv ${PERFSTAT_LIB})
    endif()

    if (${CMAKE_SYSTEM_NAME} MATCHES "FreeBSD" OR ${CMAKE_SYSTEM_NAME} MATCHES "DragonFlyBSD" OR
            ${CMAKE_SYSTEM_NAME} MATCHES "OpenBSD" OR ${CMAKE_SYSTEM_NAME} MATCHES "NetBSD")
        find_library(KVM_LIB NAMES kvm)
        target_link_libraries(uv ${KVM_LIB})
    endif()

endif()

set_target_properties(uv PROPERTIES FOLDER "Libraries")
set (LIBUV_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include" CACHE STRING "libuv" FORCE)

if(LIBUV_BUILD_TESTS)
    set (TEST_SOURCES
        test/blackhole-server.c
        test/echo-server.c
        test/run-tests.c
        test/runner.c
        test/runner.h
        test/task.h
        test/test-active.c
        test/test-async-null-cb.c
        test/test-async.c
        test/test-barrier.c
        test/test-callback-order.c
        test/test-callback-stack.c
        test/test-close-fd.c
        test/test-close-order.c
        test/test-condvar.c
        test/test-connection-fail.c
        test/test-cwd-and-chdir.c
        test/test-default-loop-close.c
        test/test-delayed-accept.c
        test/test-dlerror.c
        test/test-eintr-handling.c
        test/test-embed.c
        test/test-emfile.c
        test/test-error.c
        test/test-fail-always.c
        test/test-fs-event.c
        test/test-fs-poll.c
        test/test-fs.c
        test/test-get-currentexe.c
        test/test-get-loadavg.c
        test/test-get-loadavg.c
        test/test-get-memory.c
        test/test-get-passwd.c
        test/test-getaddrinfo.c
        test/test-getnameinfo.c
        test/test-getsockname.c
        test/test-handle-fileno.c
        test/test-homedir.c
        test/test-hrtime.c
        test/test-idle.c
        test/test-ip4-addr.c
        test/test-ip6-addr.c
        test/test-ipc-send-recv.c
        test/test-ipc.c
        test/test-list.h
        test/test-loop-alive.c
        test/test-loop-close.c
        test/test-loop-configure.c
        test/test-loop-handles.c
        test/test-loop-stop.c
        test/test-loop-time.c
        test/test-multiple-listen.c
        test/test-mutexes.c
        test/test-osx-select.c
        test/test-pass-always.c
        test/test-ping-pong.c
        test/test-pipe-bind-error.c
        test/test-pipe-close-stdout-read-stdin.c
        test/test-pipe-connect-error.c
        test/test-pipe-connect-multiple.c
        test/test-pipe-connect-prepare.c
        test/test-pipe-getsockname.c
        test/test-pipe-pending-instances.c
        test/test-pipe-sendmsg.c
        test/test-pipe-server-close.c
        test/test-pipe-set-non-blocking.c
        test/test-platform-output.c
        test/test-poll-close.c
        test/test-poll.c
        test/test-process-title.c
        test/test-queue-foreach-delete.c
        test/test-ref.c
        test/test-run-nowait.c
        test/test-run-once.c
        test/test-semaphore.c
        test/test-shutdown-close.c
        test/test-shutdown-eof.c
        test/test-shutdown-twice.c
        test/test-signal-multiple-loops.c
        test/test-signal.c
        test/test-socket-buffer-size.c
        test/test-spawn.c
        test/test-stdio-over-pipes.c
        test/test-tcp-bind-error.c
        test/test-tcp-bind6-error.c
        test/test-tcp-close-accept.c
        test/test-tcp-close-while-connecting.c
        test/test-tcp-close.c
        test/test-tcp-connect-error-after-write.c
        test/test-tcp-connect-error.c
        test/test-tcp-connect-timeout.c
        test/test-tcp-connect6-error.c
        test/test-tcp-create-socket-early.c
        test/test-tcp-flags.c
        test/test-tcp-oob.c
        test/test-tcp-open.c
        test/test-tcp-read-stop.c
        test/test-tcp-shutdown-after-write.c
        test/test-tcp-try-write.c
        test/test-tcp-unexpected-read.c
        test/test-tcp-write-after-connect.c
        test/test-tcp-write-fail.c
        test/test-tcp-write-queue-order.c
        test/test-tcp-write-to-half-open-connection.c
        test/test-tcp-writealot.c
        test/test-thread-equal.c
        test/test-thread.c
        test/test-threadpool-cancel.c
        test/test-threadpool.c
        test/test-timer-again.c
        test/test-timer-from-check.c
        test/test-timer.c
        test/test-tmpdir.c
        test/test-tty.c
        test/test-udp-bind.c
        test/test-udp-create-socket-early.c
        test/test-udp-dgram-too-big.c
        test/test-udp-ipv6.c
        test/test-udp-multicast-interface.c
        test/test-udp-multicast-interface6.c
        test/test-udp-multicast-join.c
        test/test-udp-multicast-join6.c
        test/test-udp-multicast-ttl.c
        test/test-udp-open.c
        test/test-udp-options.c
        test/test-udp-send-and-recv.c
        test/test-udp-send-immediate.c
        test/test-udp-send-unreachable.c
        test/test-udp-try-send.c
        test/test-walk-handles.c
        test/test-watcher-cross-stop.c
        )

    if(WIN32)
        set (TEST_SOURCES ${TEST_SOURCES}
            #Windows-only tests
            test/test-poll-close-doesnt-corrupt-stack.c
            test/test-poll-closesocket.c

            #Windows Runner
            test/runner-win.c
            test/runner-win.h
            )
    else()
        set (TEST_SOURCES ${TEST_SOURCES}
            #Unix runner
            test/runner-unix.c
            test/runner-unix.h
            )
    endif()

    file(COPY test/fixtures DESTINATION test)

    add_executable(run-tests ${TEST_SOURCES})
    set_property(TARGET run-tests PROPERTY C_STANDARD 99)
    target_link_libraries(run-tests uv)

    set(BENCHMARK_SOURCES
        test/blackhole-server.c
        test/dns-server.c
        test/echo-server.c
        test/run-benchmarks.c
        test/runner.c
        test/runner.h
        test/task.h
        test/benchmark-async-pummel.c
        test/benchmark-async.c
        test/benchmark-fs-stat.c
        test/benchmark-getaddrinfo.c
        test/benchmark-list.h
        test/benchmark-loop-count.c
        test/benchmark-million-async.c
        test/benchmark-million-timers.c
        test/benchmark-multi-accept.c
        test/benchmark-ping-pongs.c
        test/benchmark-pound.c
        test/benchmark-pump.c
        test/benchmark-sizes.c
        test/benchmark-spawn.c
        test/benchmark-tcp-write-batch.c
        test/benchmark-thread.c
        test/benchmark-udp-pummel.c
        )

    if(WIN32)
        set (BENCHMARK_SOURCES ${BENCHMARK_SOURCES}
            test/runner-win.c
            test/runner-win.h
            )
    else()
        set (BENCHMARK_SOURCES ${BENCHMARK_SOURCES}
            test/runner-unix.c
            test/runner-unix.h
            )
    endif()

    add_executable(run-benchmarks ${BENCHMARK_SOURCES})
    target_link_libraries(run-benchmarks uv)
endif()
