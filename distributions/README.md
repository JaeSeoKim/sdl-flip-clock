# Distribution Guide

Each directory under `distributions/` owns one target package. The core app
stays distribution-neutral: it builds `output/build/<profile>/binary/flipClock`
and only uses runtime hooks explicitly passed by the launcher.

## Responsibilities

A distribution package can provide:

- a launcher script for device-specific setup before running `flipClock`
- bundled runtime libraries when the target system does not provide SDL2
- package metadata, icons, and menu entries required by the device UI
- optional hook scripts used by the core app
- a `Makefile` that stages files and creates the final package
- a Dockerfile or toolchain wrapper when cross-compilation is needed

The root `Makefile` should only delegate to distribution targets. Packaging,
runtime libraries, Docker setup, and device-specific scripts belong inside the
distribution directory.

## Suggested Layout

```text
distributions/<name>/
  Makefile
  package/
    launch.sh
    set-time.sh     optional
  scripts/          optional build/package scripts
  make/             optional make fragments
  Dockerfile        optional
```

Generated files should go under `output/`:

```text
output/build/<profile>/binary/flipClock
output/build/<profile>/objects/
output/package/<name>/
output/distributions/<name>/
```

Do not keep built libraries or packaged files under `distributions/<name>/`.

## Launcher

Use `launch.sh` for device setup that must happen every time the app starts.
Examples:

- set `LD_LIBRARY_PATH` for bundled SDL2 libraries
- prevent device sleep while the app is running
- kill an already-running app instance
- restore temporary device state on exit

The launcher should `cd` into its own directory before running `./flipClock` so
relative assets and hook paths resolve from the packaged app directory.

## Time Hook

When the in-app time editor is applied, the core app runs a time hook only when
the launcher passes one:

```sh
./flipClock --set-time-hook ./set-time.sh
```

The hook is then called with the selected time:

```sh
./set-time.sh HH MM
```

Arguments are zero-padded 24-hour values:

```text
HH: 00-23
MM: 00-59
```

If `--set-time-hook` is not passed, no hook is used. This keeps local builds and
distributions without system-time support unchanged.

A distribution can use this hook to:

- set the system clock from the edited time
- sync the hardware clock if the device supports it
- log or ignore time changes when setting system time is not allowed

## Runtime Libraries

If the target only ships SDL1 or an incompatible SDL2 version, bundle the SDL2
runtime libraries in the package and make the launcher point `LD_LIBRARY_PATH`
to that directory. Keep those libraries in the generated package output, not in
the source tree.

## Adding A Distribution

1. Create `distributions/<name>/`.
2. Add a distribution `Makefile` with at least `package`, `stage`, `zip` or an
   equivalent final package target.
3. Build with a distribution-specific `BUILD_PROFILE`, then stage
   `output/build/<profile>/binary/flipClock`, `fonts/`, `icon/`, and
   `config.json`.
4. Add a `launch.sh` if the device needs environment setup.
5. Add optional hooks such as `set-time.sh` only when the device can support
   that behavior.
6. Put final packages under `output/distributions/<name>/`.
7. Add a small delegating target to the root `Makefile` only if the target
   should be invokable from the project root.
