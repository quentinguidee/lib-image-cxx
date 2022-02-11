# BMP File format

## Support

### Headers

|                    | Header version   |
| ------------------ | :--------------- |
| :heavy_check_mark: | `CORE_HEADER`    |
| :heavy_check_mark: | `INFO_HEADER_V1` |
| :heavy_check_mark: | `INFO_HEADER_V2` |
| :heavy_check_mark: | `INFO_HEADER_V3` |
| :heavy_check_mark: | `INFO_HEADER_V4` |
| :heavy_check_mark: | `INFO_HEADER_V5` |

### BitCount

|                    | BitCount |
| ------------------ | :------- |
| :heavy_check_mark: | `1bpp`   |
| :heavy_check_mark: | `2bpp`   |
| :heavy_check_mark: | `4bpp`   |
| :heavy_check_mark: | `8bpp`   |
| :heavy_check_mark: | `16bpp`  |
| :heavy_check_mark: | `24bpp`  |
| :heavy_check_mark: | `32bpp`  |

### Compression methods

|                    | BitCount    |
| ------------------ | :---------- |
| :heavy_check_mark:︎ | `RGB`       |
| :x:                | `RLE8`      |
| :x:                | `RLE4`      |
| :heavy_check_mark: | `BITFIELDS` |
| :x:                | `JPEG`      |
| :x:                | `PNG`       |
| :x:                | `CMYK`      |
| :x:                | `CMYK_RLE8` |
| :x:                | `CMYK_RLE4` |

### Others

|     | Features             |
| --- | :------------------- |
| :x:︎ | `ICC color profiles` |
