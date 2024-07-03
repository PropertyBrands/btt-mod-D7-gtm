<?php
/**
 * @file
 * Admin callbacks for the Google Tag Manager module.
 */

/**
 * Admin settings form.
 */
function gtm_admin_settings_form($form) {
  $user_roles = user_roles();
  $form = array();
  $form['gtm'] = array(
    '#type' => 'textfield',
    '#title' => t('Google Tag Manager ID'),
    '#default_value' => variable_get('gtm', ''),
    '#attributes' => array(
      'placeholder' => 'GTM-XXXX'
    ),
    '#size' => 10,
    '#required' => TRUE,
    '#suffix' => t(
      '<small>If you do not already have a Google Tag Manager Account, create here one: !link</small>',
      array(
        '!link' => l(
          t('Google Tag Manager'),
          'https://www.google.com/tagmanager',
          array(
            'attributes' => array(
              'target' => '_blank',
            ),
          )
        ),
      )
    ),
  );

  $form['gtm_extra'] = [
    '#type' => 'textfield',
    '#title' => t('Extra Google Tag Manager ID'),
    '#default_value' => variable_get('gtm_extra', ''),
    '#attributes' => [
      'placeholder' => 'GTM-XXXX'
    ],
    '#size' => 10,
    '#required' => false,
    '#suffix' => t(
      '<small>If you do not already have a Google Tag Manager Account, create here one: !link</small>',
      [
        '!link' => l(
          t('Google Tag Manager'),
          'https://www.google.com/tagmanager',
          [
            'attributes' => [
              'target' => '_blank',
            ],
          ]
        ),
      ]
    ),
  ];

  $form['datalayer'] = array(
    '#prefix' => '<br /><br />',
    '#type' => 'textfield',
    '#title' => t('dataLayer Variable Name'),
    '#default_value' => variable_get('datalayer', 'dataLayer'),
    '#description' => t('The variable name which should be used to instantiate the GTM dataLayer'),
    '#required' => TRUE,
    '#size' => 20,
  );

  $form['roles'] = array(
    '#type' => 'fieldset',
    '#title' => t('User Role Tracking'),
    '#collapsible' => TRUE,
    '#description' => t('Define what user roles should be tracked by Google.'),
  );
  foreach ($user_roles as $user_role) {
    $role = str_replace(' ', '_', $user_role);
    $form['roles']['gtm_track_' . $role] = array(
      '#type' => 'checkbox',
      '#title' => t('@user_role', array('@user_role' => $user_role)),
      '#default_value' => variable_get('gtm_track_' . $role, FALSE),
    );
  }

  $form['gtm_load_delay'] = [
    '#type' => 'textfield',
    // Hack to use numbers only
    // @see https://drupal.stackexchange.com/a/228250
    '#attributes' => [
      ' type' => 'number',
    ],
    '#title' => t('Load delay'),
    '#default_value' => variable_get('gtm_load_delay', 0),
    '#description' => t('Indicates the amount of miliseconds GTM lib must wait to be loaded. Useful to reduce the impact in page speed tests as GTM script loading and its referenced scripts in a site, can take some time, and that impacts scores.'),
    '#required' => false,
    '#size' => 20,
  ];

  $form['gtm_load_on_user_interaction'] = [
    '#type' => 'checkbox',
    '#title' => t('Load on user interaction'),
    '#default_value' => variable_get('gtm_load_on_user_interaction', 0),
    '#description' => t('Indicates if GTM must be loaded after detected user interaction.'),
    '#required' => false,
    '#size' => 20,
  ];

  return system_settings_form($form);
}
