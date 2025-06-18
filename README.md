# Clang Development Tool

## Статус

| master | STABLE |
|:------:|:----------:|
| [![pipeline status](https://gitlab.softcom.su/excellence-devel/clangdevelopmenttool/badges/master/pipeline.svg)](https://gitlab.softcom.su/excellence-devel/clangdevelopmenttool/-/commits/master) | [![stable status](https://gitlab.softcom.su/excellence-devel/clangdevelopmenttool/badges/STABLE/pipeline.svg?label=stable)](https://gitlab.softcom.su/excellence-devel/clangdevelopmenttool/-/commits/STABLE) |

## Building

#### Prerequisites:
 1. Java (JDK) 17+
 2. Apache Maven 3.6.3
 3. Internet access

#### Build
From the repository root

```sh
mvn verify
```
Produced p2 repository can be found at `releng/su.softcom.cldt.repository/target`
