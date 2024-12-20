## Testing the project on other distros
When in the container we reccommend to create a seperate build folder for that distro, ie build-linux.

### Linux (ubuntu)
Works easily by starting a terminal in the container
```bash
# build the image
$ docker build -t ubuntu_test linux

# run the image (with a volume of the repo)
$ docker run -it -v $PWD/../../:/play-man ubuntu_test

# Go into the volume directory
$ cd play-man
```

### Macos
Is a little bit different. Here we start macos using kvm. Which you can then connect to through the browser.
> For more information checkout [this](https://hub.docker.com/r/dockurr/macos)

```bash
# build the image
$ docker build -t macos-test macos

# run the image (with a volume of the repo)
$ docker run -p 8006:8006 --device=/dev/kvm --device=/dev/net/tun --cap-add NET_ADMIN --stop-timeout 120 macos-test
```

- After the container is started connect to localhost:8006 using your browser.

- Choose Disk Utility and then select the largest Apple Inc. VirtIO Block Media disk.

- Click the Erase button to format the disk to APFS, and give it any recognizable name you like.

- Close the current window and proceed the installation by clicking Reinstall macOS.

- When prompted where you want to install it, select the disk you just created previously.

- After all files are copied, select your region, language, and account settings.


### Windows
https://hub.docker.com/r/microsoft/windows or https://hub.docker.com/r/dockurr/windows