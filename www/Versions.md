# QtJambi Versions and Compatibility

The following table shows the incarnations of QtJambi and their relation to Qt.

A QtJambi release is published as specific source code tag. QtJambi is always based upon the latest Qt release with backward compatibility to Qt LTS versions.
The patch number always increases with the new release. Different QtJambi versions in the same row represent builds against different Qt versions but based upon the same source code.

Outgreyed version numbers are not published as Maven artifact.

| Release |  Source Tag      |                                    Qt 6.10 |                                     Qt 6.9 |                                     Qt 6.8 |                                     Qt 6.7 |                                     Qt 6.6 |                                     Qt 6.5 |
| ------- | ---------------- | ------------------------------------------ | ------------------------------------------ | ------------------------------------------ | ------------------------------------------ | ------------------------------------------ | ------------------------------------------ |
|      30 | v6.10.1          |                             6.10.1         | <span style="color:#EEEEEE">6.9.4</span>   |                             6.8.7          | <span style="color:#EEEEEE">6.7.10</span>  | <span style="color:#EEEEEE">6.6.13</span>  | <span style="color:#EEEEEE">6.5.16</span>  |
|      29 | v6.10.0 / v6.9.3 |                             6.10.0         |                             6.9.3          |                             6.8.6          | <span style="color:#EEEEEE">6.7.9</span>   | <span style="color:#EEEEEE">6.6.12</span>  | <span style="color:#EEEEEE">6.5.15</span>  |
|      28 | v6.9.2           |                                            |                             6.9.2          |                             6.8.5          | <span style="color:#EEEEEE">6.7.8</span>   | <span style="color:#EEEEEE">6.6.11</span>  | <span style="color:#EEEEEE">6.5.14</span>  |
|      27 | v6.9.1           |                                            |                             6.9.1          |                             6.8.4          | <span style="color:#EEEEEE">6.7.7</span>   | <span style="color:#EEEEEE">6.6.10</span>  | <span style="color:#EEEEEE">6.5.13</span>  |
|      26 | v6.9.0 / v6.8.3  |                                            |                             6.9.0          |                             6.8.3          | <span style="color:#EEEEEE">6.7.6</span>   | <span style="color:#EEEEEE">6.6.9</span>   | <span style="color:#EEEEEE">6.5.12</span>  |
|      25 | v6.8.2           |                                            |                                            |                             6.8.2          | <span style="color:#EEEEEE">6.7.5</span>   | <span style="color:#EEEEEE">6.6.8</span>   | <span style="color:#EEEEEE">6.5.11</span>  |
|      24 | v6.8.1           |                                            |                                            |                             6.8.1          | <span style="color:#EEEEEE">6.7.4</span>   | <span style="color:#EEEEEE">6.6.7</span>   | <span style="color:#EEEEEE">6.5.10</span>  |
|      23 | v6.8.0 / v6.7.3  |                                            |                                            |                             6.8.0          |                             6.7.3          | <span style="color:#EEEEEE">6.6.6</span>   |                             6.5.9          |
|      22 | v6.7.2           |                                            |                                            |                                            |                             6.7.2          | <span style="color:#EEEEEE">6.6.5</span>   |                             6.5.8          |
|      21 | v6.7.1           |                                            |                                            |                                            |                             6.7.1          | <span style="color:#EEEEEE">6.6.4</span>   |                             6.5.7          |
|      20 | v6.7.0           |                                            |                                            |                                            |                             6.7.0          |                             6.6.3          |                             6.5.6          |
|      19 | v6.6.2           |                                            |                                            |                                            |                                            |                             6.6.2          |                             6.5.5          |
|      18 | v6.6.1           |                                            |                                            |                                            |                                            |                             6.6.1          |                             6.5.4          |
|      17 | v6.6.0 / v6.5.3  |                                            |                                            |                                            |                                            |                             6.6.0          |                             6.5.3          |
|      16 | v6.5.2           |                                            |                                            |                                            |                                            |                                            |                             6.5.2          |
|      15 | v6.5.1           |                                            |                                            |                                            |                                            |                                            |                             6.5.1          |
|      14 | v6.5.0           |                                            |                                            |                                            |                                            |                                            |                             6.5.0          |

## Linux

Following table shows the Linux system requirement of QtJambi builds:

| QtJambi |       6.10 |        6.9 |        6.8 |
|---------|------------|------------|------------|
| x64     | GLIBC_2.29 | GLIBC_2.29 | GLIBC_2.35 |
| arm64   | GLIBC_2.35 | GLIBC_2.35 | GLIBC_2.35 |

QtJambi for Linux has been build on Ubuntu 20.04 (GLIBC_2.29) and 22.04 (GLIBC_2.35).