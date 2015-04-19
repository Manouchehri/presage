
/******************************************************
 *  Presage, an extensible predictive text entry system
 *  ---------------------------------------------------
 *
 *  Copyright (C) 2015  Matteo Vescovi <matteo.vescovi@yahoo.co.uk>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
                                                                             *
                                                                **********(*)*/


namespace presage_wcf_service
{
    public class Constants
    {
        public const string ServiceName = "Presage";
        public const string ServiceNamespace = "urn:Presage/2015/03/10";

        public const string ServiceBaseAddress = "net.pipe://localhost/PresageService/v1";
        public const string ServicePresageEndpointRelativeAddress = "presage";
        public const string ServiceMexEndpointRelativeAddress = "mex";
    }
}
