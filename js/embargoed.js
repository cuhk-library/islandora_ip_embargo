/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
(function ($, Drupal, window, document, undefined) {
    $(document).ready(function(){
        $('.object_embargo_checkbox').click(function() {
            if($('.object_embargo_checkbox').prop('checked')){
                $('.datastream_embargo_checkbox').prop('checked',false);
            }
        });
        $('.datastream_embargo_checkbox').each(function(){
            $(this).click(function(){
                if($(this).prop('checked')){
                    $('.object_embargo_checkbox').prop('checked',false);
                }
            });
        });
    })
})(jQuery, Drupal, this, this.document);

