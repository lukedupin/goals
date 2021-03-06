/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

.pragma library

function roundNumber(number, digits)
{
    var multiple = Math.pow(10, digits);
    return Math.round(number * multiple) / multiple;
}

function epochToDate(sec)
{
    var d = new Date(0)
    d.setUTCSeconds(sec)

    return (d.getMonth() + 1) +"/"+ d.getDate() +"/"+ d.getFullYear().toString().slice(-2)
}

function epochToTime(sec, short)
{
    var d = new Date(0)
    d.setUTCSeconds(sec)

    if ( short === true)
    {
        var min = d.getMinutes()
        return d.getHours() +":"+ (((min < 10)? "0": "") + min)
    }
    else
        return d.toLocaleTimeString()
}

function epochToDateTime(sec)
{
    return epochToDate(sec) +" "+ epochToTime(sec, true)
}

function distanceUnits()
{
    return "mi"
}

function distanceToUnits( meters, show_units )
{
    //This will be updated to pull from the user's settings
    if ( show_units === true )
        return (meters / 1609.34).toFixed(1) + distanceUnits()
    else
        return meters / 1609.34
}

function speedUnits()
{
    return "mph"
}

function speedToUnits( meters_per_second, show_units )
{
    //This will be updated to pull from the user's settings
    if ( show_units === true )
        return (meters_per_second * 2.23694).toFixed(1) +" "+ speedUnits()
    else
        return meters_per_second * 2.23694
}

function formatTime(sec)
{
    var value = sec
    var seconds = value % 60
    value /= 60
    value = (value > 1) ? Math.round(value) : 0
    var minutes = value % 60
    value /= 60
    value = (value > 1) ? Math.round(value) : 0
    var hours = value
    if (hours > 0) value = hours + "h:"+ minutes + "m"
    else value = minutes + "min"
    return value
}

function formatDistance(meters)
{
    var dist = Math.round(meters)
    if (dist > 1000 ){
        if (dist > 100000){
            dist = Math.round(dist / 1000)
        }
        else{
            dist = Math.round(dist / 100)
            dist = dist / 10
        }
        dist = dist + " km"
    }
    else{
        dist = dist + " m"
    }
    return dist
}
