Import("env")
from platformio.builder.tools.piolib import PackageItem
from platformio.package.version import get_original_version

print("\u001b[32m Adding platform and package version defines \u001b[0m")

platform = env.PioPlatform()
used_packages = platform.dump_used_packages()
pkg_metadata = PackageItem(platform.get_dir()).metadata
platform_version = str(pkg_metadata.version if pkg_metadata else platform.version)
##platform_version_split = platform_version.split(sep=".")

env.Append(CPPDEFINES=[
##  ("PIO_PLATFORM_VERSION_MAJOR", platform_version[0]),
##  ("PIO_PLATFORM_VERSION_MINOR", platform_version[1]),
##  ("PIO_PLATFORM_VERSION_RELEASE", platform_version[2]),
  ("PIO_PLATFORM_VERSION_FULL", "\\\"" + platform_version + "\\\"")
])

for package in used_packages:
    pio_package_version = package["version"] # e.g. "1.70300.191015"
    pio_package_name = package["name"] # e.g. "toolchain-atmelavr"
    # can fail at decoding and return None if package version is not in semver format
    # in these cases the pio package version is already the decoded version
    # e.g. 1.70300.191015 => 7.3.0
    # e.g. 5.1.0 => None
    pio_decoded_version = get_original_version(pio_package_version)
    name_converter = lambda name: name.upper().replace(" ", "_").replace("-", "_")
    print('{: <70s} = {}'.format("\u001b[33m PIO_PACKAGE_%s_PKG_VERSION\u001b[0m " % name_converter(pio_package_name), pio_package_version))

    env.Append(CPPDEFINES=[
     ("PIO_PACKAGE_%s_PKG_VERSION" % name_converter(pio_package_name) , "\\\"" + pio_package_version + "\\\"")
    ])

    if pio_decoded_version is not None: 
        print('{: <70s} = {}'.format("\u001b[33m PIO_PACKAGE_%s_DECODED_VERSION\u001b[0m " % name_converter(pio_package_name), pio_decoded_version))
        env.Append(CPPDEFINES=[
        ("PIO_PACKAGE_%s_DECODED_VERSION" % name_converter(pio_package_name) , "\\\"" + pio_decoded_version + "\\\"")
        ])