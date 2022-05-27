# teapot

A teapot implementing the [Hyper Text Coffee Pot Control Protocol (HTCPCP)](https://en.wikipedia.org/wiki/Hyper_Text_Coffee_Pot_Control_Protocol) as per RFC 2324.
Of course this is not an actual implementation (teapots aren't meant to implement this protocol), only a tribute to the 418 status code.

An Arduino Nano 33 IoT in the teapot is serving a webserver at: TODO
<p align="left">
  <img src="https://github.com/418Coffee/teapot/blob/main/teapot_small.jpeg">
</p>

## Table of contents


- [Installation](#installation)
- [Dependencies](#dependencies)
- [Contributing](#contributing)
- [License](#license)

## Installation

1. Install all [dependencies](#dependencies) using the libarary manager under `Tools`

Optional: If you wan't to add a mapping for the 418 status code you can edit [\_responseCodeToString](https://github.com/khoih-prog/WiFiWebServer/blob/master/src/WiFiWebServer-impl.h) function defined `\Arduino\libraries\WiFiWebServer\src\WifiWebServer-impl.h`

2. Define variables in `env.h`
3. Upload to arduino and visit http://192.168.2.69


## Dependencies

- [WiFiNINA_Generic](https://github.com/khoih-prog/WiFiNINA_Generic) (Tested on 1.18.4-4)
- [WifiWebServer](https://github.com/khoih-prog/WiFiWebServer) (Tested on 1.8.0)

## Contributing

Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

## License

[MIT](https://choosealicense.com/licenses/mit/)
