# GeoMessage Specification

Esri created the GeoMessage specification as a pattern for messages that are sent and received by applications and components that are part of the [ArcGIS for the Military](http://solutions.arcgis.com/military) solution. This document explains the GeoMessage format for those who want to generate, process, or otherwise use GeoMessages.

## Sections

* [XML Schema for GeoMessages](#xml-schema-for-geomessages)
* [Description of the GeoMessage Format](#description-of-the-geomessage-format)
* [Sample GeoMessages](#sample-geomessages)

## XML Schema for GeoMessages

GeoMessages are expressed in XML. A formal schema for each GeoMessage type is defined in the [schema directory](./schema/), using XML Schema Definition (XSD) files.

## Description of the GeoMessage Format

A `<geomessages>` element contains one or more `<geomessage>` elements. Each GeoMessage represents a message that a military user or system might send, like a spot report or a track report. Each `<geomessage>` element may have an attribute `v` specifying the GeoMessage version in use for that GeoMessage. If `v` is not specified, implementers could try parsers for various GeoMessage versions if available. *(Note: at the initial 1.1 release of this specification, only `v="1.1"` should be used. In some apps and samples, `v="1.0"` has been used but was never formally defined, so a GeoMessage 1.1 parser should accept both `v="1.1"` and `v="1.0"`.)*

A `<geomessage>` element contains at least the following elements:

- `<_type>`: the element value specifies the type of GeoMessage. Valid values:
    - `bedavail`: a Bed Availability and Element Status (BEDAVAIL).
    - `chemlight`: a digital chemlight.
    - `eod`: an explosive ordnance disposal (EOD) 9-line report.
    - `gsrrep`
    - `medevac`: a Medical Evacuation Request (MEDEVAC).
    - `position_report`: a Track/Point Report (TRKREP).
    - `sensor_obs`
    - `sitrep`: a Commander's Situation Report (SITREP).
    - `slantrep`: a Slant Report (SLANTREP).
    - `spotrep`: a Spot Report (SPOTREP).

## Sample GeoMessages

