<?php

/**
 * @author Gerd Riesselmann
 * @author Jeff Warrington (jaydub) is new maintainer March 2008
 * @author Chris Nutting <Chris.Nutting@openx.org>
 * @author Bruno Massa
 *
 * @file
 * Module settings.
 */

/**
 * Module settings page form.
 */
function _openx_settings($form, &$form_state) {
  $form['blocks'] = array(
    '#description'  => t('<ul>
        <li>' . t('The zone ID can be found in the OpenX administration interface.') . '</li>
        <li>' . t('The name can be any alpha-numeric string, this will be used for the block name.') . '</li>
    </ul>'),
    '#title'        => t('OpenX blocks'),
    '#type'         => 'fieldset',
    '#prefix' => '<div id="zones-wrapper">',
    '#suffix' => '</div>'
  );

  $form['blocks']['openx_zones'] = array(
    '#tree'         => TRUE,
    '#theme'        => 'openx_settings_zones',
  );

  if (empty($form_state['zones'])) {
    $zones = variable_get('openx_zones', array());
    if (empty($zones)) {
      $form_state['zones'] = array_fill(0, 3, array('id' => '', 'name' => ''));
    }
    else {
      $form_state['zones'] = $zones;
    }
  }

  foreach ($form_state['zones'] as $index => $zone) {
    $form['blocks']['openx_zones'][$index]['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#submit' => array('_openx_settings_delete_zone'),
      '#ajax' => array(
        'callback' => '_openx_settings_add_zone_callback',
        'wrapper' => 'zones-wrapper',
      ),
      '#zone_id' => $index,
      '#name' => 'delete-zone-' . $index,
    );
    $form['blocks']['openx_zones'][$index]['id'] = array(
      '#type'           => 'textfield',
      '#default_value'  => isset($form_state['zones'][$index]['id']) ? $form_state['zones'][$index]['id'] : '',
      '#size'           => 6,
    );
    $form['blocks']['openx_zones'][$index]['name'] = array(
      '#type'           => 'textfield',
      '#default_value'  => isset($form_state['zones'][$index]['name']) ? $form_state['zones'][$index]['name'] : '',
    );
  }

  $form['blocks']['add_zone'] = array(
    '#type' => 'submit',
    '#value' => t('Add zone'),
    '#submit' => array('_openx_settings_add_zone'),
    '#ajax' => array(
      'callback' => '_openx_settings_add_zone_callback',
      'wrapper' => 'zones-wrapper'
    ),
  );

  $form['adserver'] = array(
    '#collapsible'    => TRUE,
    '#collapsed'      => variable_get('openx_delivery_url', FALSE),
    '#type'           => 'fieldset',
    '#title'          => t('OpenX server'),
    '#description'    => t('These settings are located on your OpenX server at <strong>Settings -> Main Settings -> Delivery Settings</strong><br />'),
  );
  $form['adserver']['openx_delivery_url'] = array(
    '#type'           => 'textfield',
    '#default_value'  => variable_get('openx_delivery_url', 'd.openx.org'),
    '#title'          => t('OpenX delivery url'),
    '#description'    => t('For example "ads.example.org/delivery". "http://" is automatically prefixed.'),
    '#required'       => TRUE,
  );
  $form['adserver']['openx_delivery_url_https'] = array(
    '#type'           => 'textfield',
    '#default_value'  => variable_get('openx_delivery_url_https', 'd.openx.org'),
    '#title'          => t('OpenX https delivery url'),
    '#description'    => t('For example "ads.example.org/delivery". "https://" is automatically prefixed.'),
    '#required'       => TRUE,
  );

  return system_settings_form($form);
}

function _openx_settings_add_zone_callback($form, &$form_state) {
  return $form['blocks'];
}

function _openx_settings_add_zone($form, &$form_state) {
  $form_state['zones'][] = array('id' => '', 'name' => '');
  $form_state['rebuild'] = TRUE;
}

function _openx_settings_delete_zone($form, &$form_state) {
  unset($form_state['zones'][$form_state['clicked_button']['#zone_id']]);
  if (count($form_state['zones']) < 3) {
    $form_state['zones'][] = array('id' => '', 'name' => '');
  }
  $form_state['rebuild'] = TRUE;
}

/**
 * Theme the OpenX blocks list as a table
 */
function theme_openx_settings_zones($variables) {
  $form =& $variables['form'];
  $header = array(t('Zone ID'), t('Block name'), '');
  foreach (element_children($form) as $zone) {
    $rows[] = array(drupal_render($form[$zone]['id']), drupal_render($form[$zone]['name']), drupal_render($form[$zone]['delete']));
  }

  return theme('table', array('header' => $header, 'rows' => $rows));
}
