# GetChan
A simple, light, fast C program to download all images from a 4chan thread.

## Building and installing

### Dependancies

- [libcurl](https://curl.se/libcurl/) for https connections.

### Compile from source

Requirements:

- Make
- A C Compiler (tested with gcc)

To make from source, run:

```shell
git clone https://github.com/buzzqrd/getchan
cd getchan
make
```

and to install the binary:
```shell
sudo make install
```

## Usage

After installing, the easiest way to download thread images is to move into the directory where the images will be saved and then run GetChan.
```shell
cd wallpaper_downloads/
getchan [THREAD-URL]
```

To download the images to a specific path, you can do something like this:
```shell
getchan [THREAD-URL] -o ~/home/user/wallpaper_downloads/
```


## Contributing

Contributions are welcome!
- If you are fixing a bug: please describe it and include screenshots if possible.
- If you are adding a feature: please keep in mind that this is meant to be light and simple.
- If you are making the project more efficient or readable: thank you! 


## Licence

This software is licensed under [GPL-3.0](/LICENSE).

