/* -*- c++ -*- */
/*
 * @file
 * @author Piotr Krysik <ptrkrysik@gmail.com>
 * @section LICENSE
 *
 * Gr-gsm is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * Gr-gsm is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with gr-gsm; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include <stdio.h>
#include "message_printer_impl.h"
#include "grgsm/gsmtap.h"

namespace gr {
  namespace gsm {

    void message_printer_impl::message_print(pmt::pmt_t msg)
    {
        pmt::pmt_t message_plus_header_blob = pmt::cdr(msg);
        uint8_t * message_plus_header = (uint8_t *)pmt::blob_data(message_plus_header_blob);
        size_t message_plus_header_len=pmt::blob_length(message_plus_header_blob);
        
        gsmtap_hdr * header = (gsmtap_hdr *)message_plus_header;
        
        std::cout << d_prepend_string;
        for(int ii=sizeof(gsmtap_hdr); ii<message_plus_header_len; ii++)
        {
            printf(" %02x", message_plus_header[ii]);
        }
        std::cout << std::endl;
    }

    message_printer::sptr
    message_printer::make(pmt::pmt_t prepend_string)
    {
      return gnuradio::get_initial_sptr
        (new message_printer_impl(prepend_string));
    }

    /*
     * The private constructor
     */
    message_printer_impl::message_printer_impl(pmt::pmt_t prepend_string)
      : gr::block("message_printer",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(0, 0, 0))
    {
        d_prepend_string = prepend_string;
        message_port_register_in(pmt::mp("msgs"));
        set_msg_handler(pmt::mp("msgs"), boost::bind(&message_printer_impl::message_print, this, _1));

    }

    /*
     * Our virtual destructor.
     */
    message_printer_impl::~message_printer_impl()
    {
    }
  } /* namespace gsm */
} /* namespace gr */

